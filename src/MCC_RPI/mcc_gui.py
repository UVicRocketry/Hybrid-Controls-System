import PySimpleGUI as sg
import traceback
import serial.tools.list_ports
import threading
import serial
from time import sleep


class SerialConnection:
    baudrate = 9600

    def __init__(self, port):
        self.serial = serial.Serial(port, baudrate=SerialConnection.baudrate)
        self.port = port
        self.lock = threading.Lock()

    def is_connected(self):
        return self.serial.isOpen()

    def read_msg(self):
        return self.serial.readline()

    def write_msg(self, msg):
        self.lock.acquire()
        self.serial.write(f"{msg}\n".encode())
        self.lock.release()

    def close(self):
        self.serial.close()


sg.theme('Dark')


class Window(sg.Window):

    def __init__(self):
        super().__init__("Mission Control", layout, finalize=True, resizable=True)
        self.connections = {}

    def close_connections(self):
        for k, conn in self.connections.items():
            conn.close()


def refresh(window, args):
    default_serial_string = "Available Ports: \n"
    for i in serial.tools.list_ports.comports():
        default_serial_string += f"    {i.name} - {i.description}\n"

    window[KEYS.SERIAL_DISPLAY].update(default_serial_string)


def connect_to_valve_cart(window, args):
    port = args[KEYS.PORT_INPUT]
    output_key = KEYS.VALVE_CART
    start_read_thread(window, port, output_key)
    window[connect_to_valve_cart].update(disabled=True)
    start_ping_thread(window, output_key)


def connect_to_control_box(window, args):
    port = args[KEYS.PORT_INPUT]
    output_key = KEYS.CONTROL_BOX
    start_read_thread(window, port, output_key)


def connect_to_labjack(window, args):
    port = args[KEYS.PORT_INPUT]
    try:
        connection = SerialConnection(port)
    except Exception as e:
        print(e)
        traceback.print_exc()
        return
    window.connections[KEYS.LABJACK] = connection
    thread = threading.Thread(target=read_from_serial, args=[connection], daemon=True)
    thread.start()


def start_read_thread(window, port, output_key):
    try:
        connection = SerialConnection(port)
    except Exception as e:
        traceback.print_exc()
        window[output_key].write(e)
        return
    if output_key in window.connections.keys():
        window.connections[output_key].close()
    window.connections[output_key] = connection
    thread = threading.Thread(target=read_from_serial, args=[window, connection, output_key], daemon=True)
    thread.start()


def start_ping_thread(window, output_key):
    thread = threading.Thread(target=ping_thread, args=[window, output_key], daemon=True)
    thread.start()


def ping_thread(window, output_key):
    args = {KEYS.CART_INPUT: "Ping:0"}
    while output_key in window.connections.keys():
        window.write_event_value(ping_valve_cart, args)
        sleep(0.5)


def ping_valve_cart(window, args):
    write_to_valve_cart(window, {KEYS.CART_INPUT: "Ping:0"})


def read_from_serial(window, serial_connection, output_key):
    while serial_connection.is_connected():
        window[output_key].write(serial_connection.read_msg().decode())


def write_from_serial_to_file(serial_connection):
    while serial_connection.is_connected():
        print(serial_connection.read_msg().decode())


def write_to_serial(window, output_key, msg):
    connection = window.connections[output_key]
    connection.write_msg(msg)


def write_to_valve_cart(window, args):
    msg = args[KEYS.CART_INPUT]
    write_to_serial(window, KEYS.VALVE_CART, msg)
    print(f"Message sent: {msg}")


class KEYS:
    """
    Container class that holds the keys for the GUI elements
    """
    CONTROL_BOX = "control-box"
    VALVE_CART = "valve-cart"
    MISSION_CONTROL = "mission-control"
    LABJACK = "labjack"

    SERIAL_DISPLAY = "serial-display"
    PORT_INPUT = "port-input"
    CART_INPUT = "cart-input"


control_box_column = [
    [sg.Text("Control Box", font=("Arial", 20))],
    [sg.Text("Box Output", font=("Arial", 15))],
    [sg.Multiline(size=(40, 24), key=KEYS.CONTROL_BOX, background_color='black', text_color='green',
                  reroute_stdout=False, reroute_stderr=False, autoscroll=True)]
]
valve_cart_column = [
    [sg.Text("Valve Cart", font=("Arial", 20))],
    [sg.Text("Cart Output", font=("Arial", 15))],
    [sg.Multiline(size=(40, 24), key=KEYS.VALVE_CART, background_color='black', text_color='green',
                  reroute_stdout=False, reroute_stderr=False, autoscroll=True)],
    [sg.InputText(key=KEYS.CART_INPUT, size=(35, 3), do_not_clear=False)]
]

mission_control_column = [
    [sg.Text("Mission Control", font=("Arial", 20))],
    [sg.Multiline(size=(40, 24), key=KEYS.MISSION_CONTROL, background_color='black', text_color='green',
                  reroute_stdout=True, reroute_stderr=True, autoscroll=True)],
    [sg.Text("Serial Status", font=("Arial", 15))],
    [sg.Multiline(size=(40, 15), key=KEYS.SERIAL_DISPLAY, background_color='black', text_color='green')],
    [sg.Text("Port:"), sg.InputText(key=KEYS.PORT_INPUT, size=(35, 3)), sg.Button("Refresh", size=(5, 1), key=refresh)],
    [sg.Button("Valve cart", size=(7, 1), key=connect_to_valve_cart),
     sg.Button("Control Box", size=(10, 1), key=connect_to_control_box),
     sg.Button("Labjack", size=(7, 1), key=connect_to_labjack)]
]

layout = [
    [
        sg.Column(control_box_column, vertical_alignment='top'),
        sg.Column(valve_cart_column, vertical_alignment='top'),
        sg.Column(mission_control_column, vertical_alignment='top')
    ]
]


def setup_window(window):
    window.maximize()
    window[KEYS.CART_INPUT].bind("<Return>", "_Enter")
    refresh(window, None)


def handle_event(event, values, window):
    if callable(event):
        event(window, values)
    elif event == "cart-input_Enter":
        write_to_valve_cart(window, values)
    else:
        print(event, values)


def main():
    window = Window()
    setup_window(window)
    while 1:
        event, values = window.read()
        print(event)
        if event == sg.WIN_CLOSED:
            break
        try:
            handle_event(event, values, window)
        except Exception as e:
            print(e)
            traceback.print_exc()
    window.close_connections()
    window.close()


if __name__ == '__main__':
    main()
