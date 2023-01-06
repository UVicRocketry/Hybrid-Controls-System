import serial


class SerialConnection:
    baudrate = 9600

    def __init__(self, port):
        self.serial = serial.Serial(port, baudrate=SerialConnection.baudrate)
        self.port = port

    def is_connected(self):
        return self.serial.isOpen()

    def read_msg(self):
        return self.serial.readline()

    def write_msg(self, msg):
        self.serial.write(f"{msg}\n".encode())

    def close(self):
        self.serial.close()
