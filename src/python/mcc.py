
# UVR Propulsion Development Platform - Mission Control Computer Software
# Owen Crewe 2022

# This program is to be run on the MCC in the PDP system. It passes traffic from the control box to 
# the Raspberry Pi and vice versa. The control box is connected over serial The Raspberry Pi is 
# connected over a direct network connection using ethernet/optic cable.

# TODO:
# - reconnect on connection loss
#   - does a thread throwing an error get caught in a try/except?
#   - also for rpi

import logging
import threading
from time import sleep
from server import Server
from serialconnection import SerialConnection
import sys
import message

class Controller:
    '''
    Manages message buffers and forwards messages between Control Box and R-Pi
    '''
    def __init__(self, serial_port, HOST, PORT=9999, test=False):
        """
        Initializes the server and serial connections.
        :param serial_port: port for serial connection to control box
        :param HOST: IP address of the server machine
        :param PORT: port used for R-Pi <-> MCC connection, default 9999
        """
        self.server = None
        self.serial = None
        self.system_states = {
            "client_connected": False,
            "serial_connected": False
        }
        self.server_receiver_thread = None
        self.serial_receiver_thread = None
        self.server_author_thread = None
        self.serial_author_thread = None
        self.test = test

        if test:
            self.start_server(HOST, PORT)
        else:
            self.start_server(HOST, PORT)
            self.start_serial(serial_port)
    
    def start(self):
        """
        Start threads.
        :return: nothing
        """
        self.log("Starting threads.")

        if self.test:
            self.test_thread1 = threading.Thread(name="test_thread1", target=self.receiver, args=(self.server,))
            self.test_thread2 = threading.Thread(name="test_thread2", target=self.message_generator)

            self.test_thread1.daemon = False
            self.test_thread2.daemon = False

            self.test_thread1.start()
            self.test_thread2.start()
        else:
            self.server_receiver_thread = threading.Thread(name="server_receiver_thread", target=self.receiver, args=(self.server,))  # create the receiver threads
            self.serial_receiver_thread = threading.Thread(name="serial_receiver_thread", target=self.receiver, args=(self.serial,))

            self.server_author_thread = threading.Thread(name="server_author_thread", target=self.author, args=(self.serial, self.server))  # create the author threads 
            self.serial_author_thread = threading.Thread(name="serial_author_thread", target=self.author, args=(self.server, self.serial))

            self.server_receiver_thread.daemon = False  # this has to do with thread termination
            self.serial_receiver_thread.daemon = False
            self.server_author_thread.daemon = False
            self.serial_author_thread.daemon = False

            self.server_receiver_thread.start()  # start the author threads
            self.serial_receiver_thread.start()
            self.server_author_thread.start()  # start the receiver threads
            self.serial_author_thread.start()


    def close(self):
        """
        Close server and serial connections.
        :return: nothing
        """
        self.log("Closing server.")
        self.server.end_connection()
        self.log("Closed server.")
        self.log("Closing serial.")
        self.serial.close()
        self.log("Closed serial.")

    def start_server(self, HOST, PORT):
        """
        Attempts to create a server object
        :param HOST: IP address of the server machine
        :param PORT: port used for connection
        :return: Nothing
        """
        self.server = Server(HOST, PORT)
        self.server.initialize_connection()
        
        if self.server.client:
            self.log(f"{self.server.c_address} connected successfully on HOST:PORT {self.server.HOST}:{self.server.PORT}")
            self.system_states["client_connected"] = True
        else:
            self.log("Direct network connection unsuccessful. Try verifying the IP address.")
            self.server = None

    def start_serial(self, port, baudrate=9600):
        """
        Initialize serial connection and test connection
        :param port: serial port, dependent on device and port used. Eg. '/dev/ttyUSB0' for top USB port on my laptop - Owen
        :param baudrate: baudrate for serial connection. Default 9600.
        :return: Nothing
        """
        self.serial = SerialConnection(port, baudrate)
        if self.serial.ping():
            self.log(f"Serial connection successful on port {port}")
            self.system_states["serial_connected"] = True
        else:
            self.log(f"Failed to ping serial device on port {port}")
            self.serial = None

    def receiver(self, reciever):
        self.log(f"Starting {type(reciever).__name__} receiver thread.")
        while True:
            reciever.read_msg_to_queue()

    def author(self, backlog, author):
        self.log(f"Starting {type(author).__name__}  author thread.")
        while True:
            while not backlog.message_queue.empty():
                author.write_msg(str(backlog.message_queue.get()))

    def serial_test(self):
        self.log("Starting serial test thread.")
        while True:
            self.serial.read_msg_to_queue()
            msg = self.serial.message_queue.get()
            msg.setId("MCC")
            msg.setTag("FD")
            self.serial.write_msg(str(msg))

    def message_generator(self):
        self.log("Starting test author thread.")
        msg = message.Message()
        msg.setId("MCC")
        msg.setTag("FD")
        msg.setValue("N2OV", "OPEN")
        while True:
            sleep(1)
            self.log("Sending test message.")
            self.server.write_msg(str(msg))
            self.log("Sent.")


    def log(self, msg):
        logging.info(msg)

def main():
    # Logging config
    logging.basicConfig(format="%(asctime)s:%(threadName)s:%(message)s", level=logging.INFO, datefmt="%H:%M:%S")

    controller = None

    if len(sys.argv) not in (3,4):
        raise Exception("2 arguments required.\nUsage: python mcc.py <Serial Port> <Host IP> [1 (for test mode)]")

    try:
        if len(sys.argv) == 4:
            if sys.argv[3] == "1":
                controller = Controller(sys.argv[1], sys.argv[2], test=True)
        else:
            controller = Controller(sys.argv[1], sys.argv[2])
        while controller.system_states["client_connected"] == False or controller.system_states["serial_connected"] == False:
            pass
        controller.start()
    except KeyboardInterrupt: #close on ctrl+C
        logging.warning("Keyboard Interrupt")
        if controller:
            controller.close()
        logging.info("Exiting Program")
        sys.exit(1)
    # except Exception as e:
    #     logging.error(e) 
    #     sys.exit(1)

if __name__ == "__main__":
    main()