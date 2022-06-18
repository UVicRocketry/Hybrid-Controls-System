
# UVR Propulsion Development Platform - Raspberry Pi Software
# Owen Crewe 2022

# This program is to be run on the Raspberry Pi in the PDP system. It passes traffic from the control box to 
# the Raspberry Pi and vice versa. The control box is connected over serial The Raspberry Pi is 
# connected over a direct network connection using ethernet/optic cable.

# TODO:
# - reconnect on connection loss
#   - does a thread throwing an error get caught in a try/except?
#   - also for mcc

import logging
import threading
from client import Client, ConnectionFailure
from serialconnection import SerialConnection
import sys

class Controller:
    '''
    Manages message buffers and forwards messages between Control Box and R-Pi
    '''
    def __init__(self, serial_port, HOST, PORT=9999, test=False):
        """
        Initializes the client and serial connections.
        :param serial_port: port for serial connection to control box
        :param HOST: IP address of the server machine
        :param PORT: port used for R-Pi <-> MCC connection, default 9999
        """
        self.client = None
        self.serial = None
        self.system_states = {
            "client_connected": False,
            "serial_connected": False,
            "abort":False,
        }
        self.client_receiver_thread = None
        self.serial_receiver_thread = None
        self.client_author_thread = None
        self.serial_author_thread = None
        self.test = test

        if test:
            self.start_client(HOST, PORT)
        else:
            self.start_client(HOST, PORT)
            self.start_serial(serial_port)
    
    def start(self):
        """
        Start threads.
        :return: nothing
        """
        if self.test:
            self.test_thread1 = threading.Thread(name="test_thread1", target=self.receiver, args=(self.client,))
            self.test_thread2 = threading.Thread(name="test_thread2", target=self.author, args=(self.client, self.client))

            self.test_thread1.daemon = False
            self.test_thread2.daemon = False

            self.test_thread1.start()
            self.test_thread2.start()
        else:
            self.client_receiver_thread = threading.Thread(name="client_receiver_thread", target=self.receiver, args=(self.client,))  # create the receiver threads
            self.serial_receiver_thread = threading.Thread(name="serial_receiver_thread", target=self.receiver, args=(self.serial,))

            self.client_author_thread = threading.Thread(name="client_author_thread", target=self.author, args=(self.serial, self.client))  # create the author threads 
            self.serial_author_thread = threading.Thread(name="serial_author_thread", target=self.author, args=(self.client, self.serial))

            self.client_receiver_thread.daemon = False  # this has to do with thread termination
            self.serial_receiver_thread.daemon = False
            self.client_author_thread.daemon = False
            self.serial_author_thread.daemon = False

            self.client_receiver_thread.start()  # start the author threads
            self.serial_receiver_thread.start()
            self.client_receiver_thread.start()  # start the receiver threads
            self.serial_receiver_thread.start()


    def close(self):
        """
        Close client and serial connections.
        :return: nothing
        """
        self.log("Closing client.")
        self.client.end_connection()
        self.log("Closed client.")
        self.log("Closing serial.")
        self.serial.close()
        self.log("Closed serial.")

    def start_client(self, HOST, PORT):
        """
        Attempts to create a client object
        :param HOST: IP address of the client machine
        :param PORT: port used for connection
        :return: Nothing
        """
        self.client = Client(HOST, PORT)
        try:
            self.client.initialize_connection()
            self.log(f"Connected to server successfully on HOST:PORT {self.client.HOST}:{self.client.PORT}")
            self.system_states["client_connected"] = True
        except ConnectionFailure:
            self.log("Connection attempt timed out. Try again or consider validating server IP.")
            self.client = None

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

    def receiver(self, receiver, ):
        self.log(f"Starting {type(receiver).__name__} receiver thread.")
        while True:
            receiver.read_msg_to_queue()

    def author(self, backlog, author):
        self.log(f"Starting {type(author).__name__}  author thread.")
        while True:
            while not backlog.message_queue.empty():
                author.write_msg(str(backlog.message_queue.get()))

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
        sys.exit()
    except Exception as e:
        logging.error(e) 
        sys.exit()

if __name__ == "__main__":
    main()