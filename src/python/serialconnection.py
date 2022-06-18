import logging
import serial
import queue
from message import Message

# Serial port for my laptop top USB port (linux) - Owen: '/dev/ttyUSB0'

class SerialConnection:

    def __init__(self, port, baudrate):
        self.ser = serial.Serial(port, baudrate)
        self.message_queue = queue.Queue()  # used to store messages from serial

        self.ser.reset_input_buffer()

    def ping(self): # TODO
        """
        Ping serial device to test connection.
        :return: Boolean representing ping success
        """
        return True

    def write_msg(self, message):
        """
        Send message to serial device.
        :param message: string to send over serial connection.
        """
        self.ser.write(message.encode())

    # TODO: 
    # - retransmit request on invalid msg
    #   - look into serial dataloss handling
    def read_msg_to_queue(self):
        """
        Read a message from the serial buffer.
        :return: High priority message
        """
        while self.ser.in_waiting:
            pass
        msg = self.ser.readline().decode()
        logging.info(msg)
        msg = Message(msg)
        if "ABORT" in msg.getLabels():
            with self.message_queue.queue.mutex:
                self.message_queue.clear()
        logging.info(msg)
        self.message_queue.put(msg)

    def close(self):
        self.ser.close()