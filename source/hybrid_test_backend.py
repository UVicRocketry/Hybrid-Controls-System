from PyQt5 import QtWidgets, uic, QtCore
from PyQt5.QtCore import QTimer, QDateTime, QThread, pyqtSignal
import sys
import server
HOST = '192.168.0.117'
PORT = 9999


class GetStatusThread(QThread):

    """
    Thread that is in charge of constantly checking to see if we have recieved any data from the client
    If so, pop off the data and change our states
    """

    status_signal = pyqtSignal(object)

    def __init__(self, server_ob, states):
        """
        Initializes the thread
        :param server_ob: the object representing the connection to the server
        :param states: the dictionary of states
        """
        self.client_server = server_ob
        self.states = states
        QThread.__init__(self)

    def __del__(self):
        self.wait()

    def run(self):
        """
        Runs in a thread to constantly provide status updates from the client
        Is in charge of updating the status messages shown on the GUI
        """
        while True:

            try:
                self.client_server.receive_states() # Will hang up on this line until it receives something from client
            except server.NoConnection:
                return
            param = None
            state = None
            while self.client_server.feedback_queue.qsize() > 0:    # clears the queue and pops output to GUI

                try:
                    token = self.client_server.feedback_queue.get(True, 3)
                    param = token[0]
                    state = token[1]
                except server.queue.Empty: # if we have a mix up with our queue something bad has happened
                    self.status_signal.emit(f'Failed to parse arguments: {param}, {state}')
                    continue
                except IndexError:
                    self.status_signal.emit(f'Failed to parse arguments: {param}, {state}')
                    self.status_signal.emit(f'Reattempting...')
                    continue

                if param not in self.states.keys():  # checks to make sure parameter is valid
                    self.status_signal.emit(f'Invalid parameter {param}')
                    continue

                self.states[param] = state
                self.status_signal.emit(f'{param} {state}')


class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
        # Load the UI Page. uic is the thing that lets us use a .ui file
        # This only works if the .ui file is in the same directory
        super(MainWindow, self).__init__(*args, **kwargs)
        uic.loadUi('new_hybrid_controls_gui.ui', self)
        
        self.setup_buttons()
        self.setup_timers()
        self.client_server = None
        self.status_thread = None

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
        # MANUAL CHECKBOXES
            # checkboxes used ".isChecked" to singal a change in state. 
            # Each function connected will check the current state of the switch.
            
        self.ignitor_man.stateChanged.connect(self._Ignitor_btn)
        self.mev_man.stateChanged.connect(self._MEV_btn)
        self.n2o_vent_man.stateChanged.connect(self._N2OV_btn)
        self.n2o_flow_man.stateChanged.connect(self._N2O_btn)
        self.n2_flow_man.stateChanged.connect(self._N2_btn)
        self.n2_vent_man.stateChanged.connect(self._N2V_btn)
        self.ncv_man.stateChanged.connect(self._NCV_btn)
        self.purge_vent_man.stateChanged.connect(self._RV_btn)
        # AUTO CHECKBOXES
        self.purge_vent_auto.stateChanged.connect(self._RV_btn)
        self.n2o_vent_auto.stateChanged.connect(self._N2OV_btn)
        self.n2o_flow_auto.stateChanged.connect(self._N2O_btn)
        self.n2_flow_auto.stateChanged.connect(self._N2_btn)
        self.n2_vent_auto.stateChanged.connect(self._N2V_btn)
        # self.VV_btn_off.stateChanged.connect(self._VV_btn)#not in new gui TODO: look into why not in new gui

       
        # self.n2o_flow_auto.isChecked.connect(self.)
        # self.n2_flow_auto.isChecked.connect(self.)



        self.abort_btn.clicked.connect(self._abort_btn)
        self.vent_all_btn.clicked.connect(self._vent_all_btn)
        self.fire_btn.clicked.connect(self._run_btn)


    def setup_timers(self):
        # See setup_buttons() for more details. We're doing the same
        # thing but first creating a timer and connecting it's .timeout signal
        # to a method that runs some code.

        # Timer to update the "Date/Time" QlineEdit with current system time
        self.date_time_timer = QTimer()
        self.date_time_timer.timeout.connect(self._date_time)
        self.date_time_timer.start(1000)

    ############################# BACKEND ###############################


    # Create an instance of the Server class. This is what will allow our
    # communication between the backend and the server   
    def _connect_btn(self):
        # Get our IP from the box on the GUI, instantiate a Server with it
        # PORT is hard coded in at the top of the file, if you change PORT here, you must also change
        # it in client
        if self.client_server is not None:
            if self.client_server.client is not None:
                return

        if self.lineEdit_IPaddress.text() == "":   # this if statement checks to see if we haven't entered an IP
            self.add_system_status("Invalid IP")
            return
        try:
            self.client_server = server.Server(self.lineEdit_IPaddress.text(), PORT)
            self.client_server.initialize_connection()
        except Exception as e:
            print(f'line 171 {e}')


        # Attempt connection to the Server on the rPi
        if(self.client_server.client != None):
            self.add_system_status(f"Connection Successful on HOST:PORT {self.client_server.HOST}:{self.client_server.PORT}") 
            self.system_states["connected"] = True
            self.send_states("connected True")
            try:
                self.status_thread = GetStatusThread(self.client_server, self.system_states)
                self.status_thread.status_signal.connect(self.add_system_status)
                self.status_thread.start()
            except Exception as e:
                print(f'line 184 {e}')
        else:
            self.add_system_status("Connection Unsuccessful. Is the IP address correct?")
            self.client_server = None

    # Disconnect from the Server on the rPi
    def _disconnect_btn(self):
        try:
            self.send_states("connected false")
            self.client_server.end_connection()
            self.add_system_status("Disconnection Successful")
            self.system_states["connected"] = False
        
        # This error throws if you try to disconnect before making a connection
        except AttributeError:
            self.add_system_status("Disconnection Unsuccessful. Does the connection exist?")

   
   
    # All these functions do the basically the same thing
        # Add a message to the "System Status" panel on the GUI.
        # Update our system_states dictionary
        # Send the new state over the client_server Server object
    def _igniter_btn_on(self):#TODO: ignitor needs reworking talk to connor... 
        self.add_system_status("Igniting")
        self.system_states["igniter"] = True
        self.send_states("igniter True")
       
    def _igniter_btn_off(self):
        self.add_system_status("Un-igniting?")
        self.system_states["igniter"] = False
        self.send_states("igniter False")

    def _MEV_btn_off(self):  
        self.add_system_status("Closing MEV")
        self.system_states["MEV"] = "closed"
        self.send_states("MEV closed")

    def _MEV_btn_on(self):
        self.add_system_status("Opening MEV")
        self.system_states["MEV"] = "open"
        self.send_states("MEV open")

    def _N2OV_btn_off(self):
        self.add_system_status("Closing N2O Vent")
        self.system_states["N2OV"] = "closed"
        self.send_states("N2OV closed")
        
    def _N2OV_btn_on(self):
        self.add_system_status("Opening N2O Vent")
        self.system_states["N2OV"] = "open"
        self.send_states("N2OV open")

    def _N2O_btn_off(self):
        self.add_system_status("Closing N2O Valve")
        self.system_states["N2O"] = "closed"
        self.send_states("N2O closed")

    def _N2O_btn_on(self):
        self.add_system_status("Opening N2O Valve")
        self.system_states["N2O"] = "open"
        self.send_states("N2O open")

    def _N2_btn_off(self):
        self.add_system_status("Closing N2 Valve")
        self.system_states["N2"] = "closed"
        self.send_states("N2 closed")

    def _N2_btn_on(self):
        self.add_system_status("Opening N2 Valve")
        self.system_states["N2"] = "open"
        self.send_states("N2 open")

    def _N2V_btn_off(self):
        self.add_system_status("Closing N2 Vent Valve")
        self.system_states["N2V"] = "closed"
        self.send_states("N2V closed")

    def _N2V_btn_on(self):
        self.add_system_status("Opening N2 Vent Valve")
        self.system_states["N2V"] = "open"
        self.send_states("N2 open")

    def _NCV_btn_off(self):
        self.add_system_status("Closing NC Valve")
        self.system_states["NCV"] = "closed"
        self.send_states("NCV closed")

    def _NCV_btn_on(self):
        self.add_system_status("Opening NC Valve")
        self.system_states["NCV"] = "open"
        self.send_states("NCV open")

    def _RV_btn_off(self):
        self.add_system_status("Closing Relief Valve")
        self.system_states["RV"] = "closed"
        self.send_states("RV closed")

    def _RV_btn_on(self):
        self.add_system_status("Opening Relief Valve")
        self.system_states["RV"] = "open"
        self.send_states("RV open")

    def _VV_btn_off(self):
        self.add_system_status("Closing Vent Valve")
        self.system_states["VV"] = "closed"
        self.send_states("VV closed")

    def _VV_btn_on(self):
        self.add_system_status("Opening Vent Valve")
        self.system_states["VV"] = "open"
        self.send_states("VV open")

    def _abort_btn(self):
        self.add_system_status("ABORTING")
        self.system_states["abort"] = True
        self.send_states("abort True")

    def _run_btn(self):
        self.add_system_status("RUNNING")
        self.system_states["run"] = True
        self.send_states("run True")
    #auto and manual toggle buttons
    #The filling and Venting portions will  
    #All these functions check the current state of the checkbox and run the corresponding function above.
    # def _igniter_btn_toggle(self):
    #     if self.igniter_man.isChecked() == True:
    def _Ignitor_btn(self):
        if self.ignitor_man.isChecked() == True:
            self._igniter_btn_on()
        else:
            self._igniter_btn_off()
    def _MEV_btn(self):
        if self.mev_man.isChecked() == True:
            self._MEV_btn_on()
        else:
            self._MEV_btn_off()

    def _N2OV_btn(self):
        if self.n2o_vent_man.isChecked() == True or self.n2o_vent_auto.isChecked() == True:
            self._N2OV_btn_on()
        else:
            self._N2OV_btn_off()
    def _N2O_btn(self):
        if self.n2o_flow_man.isChecked() == True or self.n2o_flow_auto.isChecked() == True:
            self._N2O_btn_on()
        else:
            self._N2O_btn_off()
   
    def _N2V_btn(self):
        if self.n2_vent_man.isChecked() == True or self.n2_vent_auto.isChecked() == True:
            self._N2V_btn_on()
        else:
            self._N2V_btn_off() 
    def _N2_btn(self):
        if self.n2_flow_man.isChecked() == True or self.n2_flow_auto.isChecked() == True:
            self._N2_btn_on()
        else:
            self._N2_btn_off()
    
    def _NCV_btn(self):
        if self.ncv_man.isChecked() == True:
            self._NCV_btn_on()
        else:
            self._NCV_btn_off()
    def _RV_btn(self):
        if self.purge_vent_auto.isChecked() == True or self.purge_vent_man == True:
            self._RV_btn_on()
        else:
            self._RV_btn_off()

    #auto control buttons
    #MAIN control buttons
    def _vent_all_btn(self): #TODO: might need to change
        self.add_system_status("Vent Valve (vent all)")
        toggle = self.system_states["VV"]
        toggle = not bool(toggle)
        self.system_states["VV"] = toggle
        self.send_states(f"{toggle} Vent Valve (vent all)")

    def _run_btn(self):
        self.add_system_status("RUNNING")
        self.system_states["run"] = True
        self.send_states("run True")

    def _abort_btn(self):
        self.add_system_status("ABORTING")
        self.system_states["abort"] = True
        self.send_states("abort True")
    


    #SYSTEM PARAMETERS 
        # MANUAL and AUTO Buttons
        #these buttons will disable the other control category 
        #if Manual is toggled on then the auto buttons will be disabled
        
        #Load test

        #Edit line parameters





    # The "System Status" box on the gui is a QPlainTextEdit that we can add
    # text to. It will contain a log of all the things that have happened in the gui
    def add_system_status(self, msg):
        self.statusbox.appendPlainText(msg)


    ############################### METHODS ON TIMERS ##################################
    def _date_time(self):
        self.date_time.setText(QDateTime.currentDateTime().toString())

    ############################### SERVER COMMUNICATION METHODS ##################################

    def send_states(self, msg):
        """
        Sends a message from the server to the client

        msg: The message being sent, message is of the form "system_property state"
             for example "abort True"
        """
        if self.client_server is None:
            self.add_system_status("ERROR[1]: Connection Not Initialized")
            return
        try:
            self.client_server.send_states(msg)
        except WindowsError as e:
            self.add_system_status(f"ERROR[1]: Connection Not Initialized {e}")
            self.client_server = None

def main():
    app = QtWidgets.QApplication(sys.argv)
    main = MainWindow()
    main.show()
    sys.exit(app.exec_())

if __name__ == '__main__':      
    main()
