from PyQt5 import QtWidgets, uic, QtCore
from PyQt5.QtCore import QTimer,QDateTime
import sys
import server

class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
        # Load the UI Page. uic is the thing that lets us use a .ui file
        # This only works if the .ui file is in the same directory
        super(MainWindow, self).__init__(*args, **kwargs)
        uic.loadUi('hybrid_test_gui.ui', self)
        
        self.setup_buttons()
        self.setup_timers()

        ############################ STATES ###############################
        # These will get updated by the GUI if the user clicks a button or
        # if the server changes something. Any time one of these is changed,
        # it will be sent over to the server using send_states()
        self.system_states = {
            "connected" : False,
            "igniter" : False,
            "MEV" : "closed",
            "N2OV" : "closed",
            "N2O" : "closed",
            "N2" : "closed",
            "NCV" : "closed",
            "RV" : "closed",
            "VV" : "closed",
            "abort" : False,
            "run" : False
        }

        

    ################################ SETUP ####################################

    def setup_buttons(self):
        # Alright so basically since there is not a "loop" to put methods in that
        # you want to update based on things that have changed in the GUI,
        # PyQt has these things called signals and slots. They let you connect
        # changes or "signals" of objects in the GUI to methods.

        # For instance we can connect_btn is a QPushButton from QT Designer, 
        # which has a signal "clicked". We can "connect" this to a method that
        # we want to run when the button is clicked. 
        self.connect_btn.clicked.connect(self._connect_btn)

        # We do this for every button in the GUI. Each button gets a corresponding
        # method that has the same name as the button but with an _ in front
        
        # To add a new button:
            # Add the button to the GUI in QT Designer
            # Give it a nice name
            # Add a new line to this method in the form "self.button_name.clicked.connect(self._button_name)"
            # Add a new method in below setup_button_etc of the form "def _button_name(self):"
            # Any code in that method will be run when the button is clicked!
        self.disconnect_btn.clicked.connect(self._disconnect_btn)
        self.igniter_btn_toggle.clicked.connect(self._igniter_btn_toggle)
        self.MEV_btn_off.clicked.connect(self._MEV_btn_off)
        self.MEV_btn_on.clicked.connect(self._MEV_btn_on)
        self.N2OV_btn_off.clicked.connect(self._N2OV_btn_off)
        self.N2OV_btn_on.clicked.connect(self._N2OV_btn_on)
        self.N2O_btn_off.clicked.connect(self._N2O_btn_off)
        self.N2O_btn_on.clicked.connect(self._N2O_btn_on)
        self.N2_btn_off.clicked.connect(self._N2_btn_off)
        self.N2_btn_on.clicked.connect(self._N2_btn_on)
        self.NCV_btn_off.clicked.connect(self._NCV_btn_off)
        self.NCV_btn_on.clicked.connect(self._NCV_btn_on)
        self.RV_btn_off.clicked.connect(self._RV_btn_off)
        self.RV_btn_on.clicked.connect(self._RV_btn_on)
        self.VV_btn_off.clicked.connect(self._VV_btn_off)
        self.VV_btn_on.clicked.connect(self._VV_btn_on)
        self.abort_btn.clicked.connect(self._abort_btn)
        self.run_btn.clicked.connect(self._run_btn)

    def setup_timers(self):
        # Timer to update the "Date/Time" QlineEdit with current system time
        self.date_time_timer = QTimer()
        self.date_time_timer.timeout.connect(self._date_time)
        self.date_time_timer.start(1000)

    ############################# BACKEND ###############################


    # Create an instance of the Server class. This is what will allow our
    # communication between the backend and the server   
    def _connect_btn(self):
        # Get our IP from the box on the GUI, instantiate a Server with it
        self.client_server = server.Server(self.lineEdit_IPaddress.text(), 9999)

        # Attempt connection to the Server on the rPi
        if(self.client_server.client != None):
            self.add_system_status(f"Connection Successful on HOST:PORT {self.client_server.HOST}:{self.client_server.PORT}") 
            self.system_states["connected"] = True
            self.client_server.send_states("connected True")
        else:
            self.add_system_status("Connection Unsuccessful. Is the IP address correct?")
    

    # Disconnect from the Server on the rPi
    def _disconnect_btn(self):
        try:
            self.client_server.end_connection() # TODO This always fails for some reason.
            self.add_system_status("Disconnection Successful")
            self.system_states["connected"] = False
        
        # This error throws if you try to disconnect before making a connection
        except AttributeError:
            self.add_system_status("Disconnection Unsuccessful. Does the connection exist?")

    # Turns the igniter on or off.
    def _igniter_btn_toggle(self):
        # This whacky bit of code is because the button is a toggle
        toggle = self.system_states["igniter"]
        toggle = not bool(toggle)
        self.client_server.send_states(f"igniter {toggle}")
        if(toggle):
            self.add_system_status("Igniting!")
        else:
            self.add_system_status("Un-igniting!")

    def _MEV_btn_off(self):  
        print("Hello World")

    def _MEV_btn_on(self):
        print("Hello World")

    def _N2OV_btn_off(self):
        print("Hello World")
        
    def _N2OV_btn_on(self):
        print("Hello World")

    def _N2O_btn_off(self):
        print("Hello World")

    def _N2O_btn_on(self):
        print("Hello World")

    def _N2_btn_off(self):
        print("Hello World")

    def _N2_btn_on(self):
        print("Hello World")

    def _NCV_btn_off(self):
        print("Hello World")

    def _NCV_btn_on(self):
        print("Hello World")

    def _RV_btn_off(self):
        print("Hello World")

    def _RV_btn_on(self):
        print("Hello World")

    def _VV_btn_off(self):
        print("Hello World")

    def _VV_btn_on(self):
        print("Hello World")

    def _abort_btn(self):
        print("Hello World")

    def _run_btn(self):
        print("Hello World")


    # The "System Status" box on the gui is a QPlainTextEdit that we can add
    # text to. It will contain a log of all the things that have happened in the gui
    def add_system_status(self, msg):
        self.statusbox.appendPlainText(msg)

    ############################### METHODS ON TIMERS ##################################
    def _date_time(self):
        self.date_time.setText(QDateTime.currentDateTime().toString())

    

def main():
    app = QtWidgets.QApplication(sys.argv)
    main = MainWindow()
    main.show()
    sys.exit(app.exec_())

if __name__ == '__main__':      
    main()
