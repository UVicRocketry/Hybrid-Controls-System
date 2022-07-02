import logging
import socket
import queue
from message import Message


class NoConnection(Exception):
    """
    A custom exception raised when attempting to utilize the server when no connection currently exists
    """
    def __str__(self):
        return "NoConnection"


class InvalidIp(Exception):
    """
    A custom exception raised when an invalid IP is provided when creating the server
    """
    def __str__(self):
        return "InvalidIp"


class Server:

    def __init__(self, host, port):
        """
        Initializes the server object
        Creates an address tuple from the given host and port
        :param host: The IP of the server, will be received form the GUI
        :param port: The port that the socket will be connecting on, is hard coded to 9999
        """
        self.HOST = host  # the host IP
        self.PORT = port  # the host port
        self.address = (self.HOST, self.PORT)  # a tuple used by the socket object
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # the server socket object
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)   # allows the server and client to reconnect
        self.client = None  # just a placeholder for now, becomes the client when a successfully connected
        self.c_address = None  # just a placeholder for now, becomes the client when a successfully connected
        self.message_queue = queue.Queue()  # used to store messages from client


    def initialize_connection(self):
        """
        Binds the server socket to the address
        Attempts to connect to the client
        Once connected self.client refers to the client socket
        self.c_address is the address of the client
        :return:
        """
        try:
            self.server.bind(self.address)  # binds the server object to the HOST and PORT
        except socket.gaierror: # this is an exception raised if the ip is invalid
            raise InvalidIp
        logging.info("Waiting for client to connect.")
        self.server.listen(5)  # waits for a connection from the client
        self.client, self.c_address = self.server.accept()  # when a successful connection is made

    def end_connection(self):
        """
        cleans up the sockets when we are done with them, leaving them open can cause problems
        :return: nothing
        """
        try:
            self.client.close()
            self.server.close()
        except AttributeError:
            logging.error('[Server] Connection already closed.')

        self.server = None
        self.client = None
        logging.info("[Server] Successfully closed connection.")

    def write_msg(self, msg):
        """
        Send msg to client
        :param msg: The state being sent
        :return: nothing
        """
        try:
            logging.info("Sending" + msg)
            self.client.sendall((msg).encode())  # send off the data
        except socket.error:  # this will fail if there is no connection initialized
            raise NoConnection

    def read_msg_to_queue(self):
        """
        Receive data from client and generates a Message object, then add it to the instruction queue
        :return: High priority message
        """
        if self.client is None:
            raise NoConnection
        try:
            msg = Message(self.client.recv(1024).decode()) # receives data which it decodes() into a string
            logging.info(msg)
            if "ABORT" in msg.getLabels():
                with self.message_queue.queue.mutex:
                    self.message_queue.clear()
            self.message_queue.put(msg)  # adds message to the queue
        except socket.error as e:
            logging.error(f'read_msg_to_queue: {e}')
            raise NoConnection
        return None
