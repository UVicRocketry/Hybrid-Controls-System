import serial

# Serial port for my laptop top USB port (Pop!_OS) - Owen: '/dev/ttyUSB0'

class SerialConnection:

    def __init__(self, port, baudrate):
        self.ser = serial.Serial(port, baudrate, timeout=0.1)

    def write_msg(self, msg):
        pass

