import socket
import queue

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 9999       # The port used by the server


class Client:

    def __init__(self, host, port):
        """
        Initializes the client object
        Creates an address tuple from the given host and port
        :param host: The IP of the client, IS HARD CODED, NEED TO CHANGE IF IP OF SERVER CHANGES
        :param port: The port that the socket will be connecting on. Hard coded to 9999
        """
        self.host = host  # The ip of the server that the client connects to
        self.port = port  # The port the client connects on
        self.address = (self.host, self.port)  # A tuple formed from the given host and ip, used for socket object
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Initializes the client socket
        self.feedback_queue = queue.Queue()  # A queue used to store the incoming instructions from the server

    def initialize_connection(self):
        """
        Connects the the server. The server must be running first in order to make this work
        """
        self.client.connect(self.address)   # connects the client to the server, server must be running first

    def end_connection(self):
        """
        Cleans up the socket objects
        """
        self.client.close() # cleans up the socket

    def send_states(self, data):
        """
        Send state to server, used when input is received from control software
        :param data: The state being sent
        :return: nothing
        """
        self.client.sendall(data.encode())

    def receive_states(self):
        """
        Receive data from server and tokenize it, then add it to the instruction queue
        :return: nothing
        """
        data = self.client.recv(1024).decode()  # receives data which it decodes() into a string
        data = data.split(" ")  # splits string into a list using spaces as the delimiter
        for i in data:  # loops through the created list
            self.feedback_queue.put(i)  # adds each list entry to the queue



if __name__ == '__main__':
    c = Client(HOST, PORT)
    c.initialize_connection()
    while True:

        c.receive_states()

        while c.feedback_queue.qsize() > 0:
            d = c.feedback_queue.get()
            if d == 'connected':
                print(d)
                d = c.feedback_queue.get()
                if d == 'false':
                    print(d)
                    exit()
            print(d)
        c.send_states('test response')



