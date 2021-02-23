"""
This file takes input from the engine stepper motors and other elements
from the PCB and PI and decodes the data into information that the GUI back end
can read and send it through client.py. 
"""
#Throwing strings as communication protocol 
#
#assume everything is a dictionary
# 

import client
import threading
from serial import Serial

baud_rate = 115200          # In the arduino .ino file, Serial.begin(baud_rate)
serial_port = "dev/ttyUSB0" # Something similar to this. It will depend what usb port the arduino is connected to
#ser = Serial(serial_port, baud_rate)


HOST = '127.0.0.1' # depends on IP if the server double check this for connection purposes
PORT = 9999 # the port that the server is using


class Receiver:
    """
    Receives data from server and passes instructions to controller
    """
    def __init__(self, controller):
        """
        Initializes the receiver and binds the client to the address, then attempts a connection
        """
        self.client = client.Client(HOST, PORT)
        self.ctrl = controller
        self.receive_thread = None # Will become the receiver thread


    def start(self):
        self.conn_attempt()  # attempts to initalize connection
        self.ctrl.client = self.client

        self.receive_thread = threading.Thread(target=self.receive_instructions())  # create the receiver thread
        self.receive_thread.daemon = False  # this has to do with thread termination
        self.receive_thread.start()  # start the receiver thread

    def conn_attempt(self):
        """
        attempts to make a connection to a server object
        :return: Nothing
        """
        while True:
            try:
                self.client.initialize_connection()  # attempts to make connection
                return # if it works
            except client.ConnectionFailure as e:  # if it times out try again
                print(str(e) + ' conn_attempt')

    def receive_instructions(self):
        """
        will run in a receiver thread and will constantly check for updates from the server
        :return: Nothing
        """

        while True:

            try:
                self.client.receive_states()  # will hang up on this line until instructions are received
            except client.NoConnection:
                self.fail_state('No Connection')
                return

            while self.client.feedback_queue.qsize() > 0:  # if there are instructions in the queue
                token = self.client.feedback_queue.get(True, 3)
                param = token[0] # parse the instruction in terms of parameter and state
                state = token[1]

                if param == "connected" and state == "False": # checks if connection is ending
                    self.fail_state('Connection Ended')
                    return

                self.ctrl.write_to_serial(f'{param} {state}')  # send the command to our controller

    def fail_state(self, error):
        try:
            self.ctrl.abort()
        except Exception as e:
            print(e)
            print("Well, this isn't good, guess we'll die")
        print(str(error) + " fail_state")
        try:
            #self.receive_thread.join()
            self.client.end_connection()
        except TypeError as e:
            print(e)


class Controller:

    def __init__(self):
        self.system_states = {
            "connected": False,
            "igniter": False,
            "MEV": "closed",
            "N2OV": "closed",
            "N2O": "closed",
            "N2": "closed",
            "NCV": "closed",
            "RV": "closed",
            "N2V": "closed",
            "BD": 0,
            "MEVo%": 0,
            "ID": 0,
            "MEVoSP": 0
        }

        self.abort_states = {
            "igniter": False,
            "MEV": "closed",
            "N2OV": "open",
            "N2O": "closed",
            "N2": "closed",
            "NCV": "closed",
            "RV": "open",
            "N2V": "open",
            "BD": 0,
            "MEVo%": 0,
            "ID": 0,
            "MEVoSP": 0
        }

        self.client = None

    def write_to_serial(self, command):
        try:
            self.read_from_serial(command)
            #ser.writelines(command.encode())  # Write stuff to arduino
        except Exception as e:
            print(str(e) + ' write to serial')

    def read_from_serial(self, command):
        try:
            command = command.split()
            self.system_states[command[0]] = command[1]

            #command = ser.readline().decode().split
            #self.system_states[command[0]] = command[1]  # update our dictionary
            self.state_update(command[0])  # send updated dictionary back to server
        except Exception as e:
            print(str(e) + ' read from serial')


    def state_update(self, command):

        self.client.send_states(f"{command} {self.system_states[command]}")

    def abort(self):
        for i in self.abort_states:
            self.write_to_serial(f'{i}, {self.abort_states[i]}')


def main():
    controller = Controller()
    receiver = Receiver(controller)
    try:
        receiver.start()
    except Exception as e:
        receiver.fail_state(e)
        print(str(e) + ' main failure')


if __name__ == '__main__':
    main()