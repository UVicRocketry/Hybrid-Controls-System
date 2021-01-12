"""
This file takes input from the engine stepper motors and other elements
from the PCB and PI and decodes the data into information that the GUI back end
can read and send it through a Client object from client.py.
"""

import client
import threading

HOST = '192.168.0.124' # depends on IP of the server double check this for connection purposes
PORT = 9999 # the port that the server is using


class Receiver:
    """
    Receives data from server and passes instructions to controller
    """
    def __init__(self):
        """
        Initializes the receiver and binds the client to the address, then attempts a connection
        """
        self.client = client.Client(HOST, PORT)  # creates a client object that will connect to the server
        self.states = {
            "connected": False,
            "igniter": False,
            "MEV": "closed",
            "N2OV": "closed",
            "N2O": "closed",
            "N2": "closed",
            "NCV": "closed",
            "RV": "closed",
            "VV": "closed",
            "abort": False,
            "run": False
        }  # initializes dictionary of states that will be modified by input from the server

        self.conn_attempt()  # attempts to initalize connection

        self.recieve_thread = threading.Thread(target=self.receive_instructions())  # create the receiver thread
        self.recieve_thread.daemon = False  # this has to do with thread termination
        self.recieve_thread.start()  # start the receiver thread

    def conn_attempt(self):
        """
        attempts to make a connection to a server object, will stay here until a connection is successful, so make sure
        to run it in a thread if you don't want your program to freeze here
        :return: None
        """
        while True:
            try:
                self.client.initialize_connection()  # attempts to make connection
                return # if it works
            except client.ConnectionFailure:  # if it times out try again
                pass
            except Exception as e:  # checks for unhandled exception
                print(e)  # if this is still just a print statement when you see this, it should not be

    def receive_instructions(self):
        """
        will run in a receiver thread and will constantly check for updates from the server
        :return: Nothing
        """

        while True:

            self.client.receive_states()  # will hang up on this line until instructions are received

            while self.client.feedback_queue.qsize() > 0:  # if there are instructions in the queue
                token = self.client.feedback_queue.get(True, 3)  # get the instructions
                # the instruction should be a tuple of length 2
                # keeping each token self contained prevents possible errors with multiple programs
                param = token[0] # parse the instruction in terms of parameter and state
                state = token[1]
                self.states[param] = state  # set our dictionary


class Controller:

    pass


def main():
    pass


if __name__ == '__main__':
    main()
