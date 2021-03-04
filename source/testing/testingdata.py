from serial import Serial

arduino = Serial("COM5", 115200)
# arduino.open()

def write_to_serial(command):
    arduino.writelines(command.encode("utf-8"))
    print(command)

write_to_serial("MEV_open")