import threading
import server
# import serial TODO: read/write from important buttons box

PORT = 9999 #Arbitrary port

#TODO: change add_system_status, maybe logging library? sounds like it could be good

class Receiver:
    """
    Receives data from client and passes instructions to controller
    """
    def __init__(self, controller):
        """
        Initializes the receiver and server
        """
        self.server = None
        self.ctrl = controller
        self.receive_thread = None # Will become the receiver thread
        ############################ STATES ###############################
        # These are the states. TODO: figure out what is needed here
        self.system_states = {
            "connected" : False,
            "igniter" : False,
            "MEV" : "closed",
            "N2OV" : "closed",
            "N2O" : "closed",
            "N2" : "closed",
            "NCV" : "closed",
            "RV" : "closed",
            "N2V" : "closed",
            "abort" : False,
            "run" : False,
            "BD" : 0, #TODO: figure out default values from Morgan 
            "MEVo%" : 0,
            "ID" : 0,
            "MEVoSP": 0
        }


    def start(self):
        self.conn_attempt()  # attempts to initalize connection
        self.ctrl.client = self.client

        self.receive_thread = threading.Thread(target=self.receive_instructions())  # create the receiver thread
        self.receive_thread.daemon = False  # this has to do with thread termination
        self.receive_thread.start()  # start the receiver thread

    def start_server(self, HOST):
        """
        attempts to create a server object
        :return: Nothing
        """
        self.server = server.Server(HOST, PORT)
        self.server.initialize_connection()
        
        if self.server.client:
            self.add_system_status(f"Connection Successful on HOST:PORT {self.server.HOST}:{self.server.PORT}")
            self.system_states["connected"] = True
            self.send_states("connected True")
            try:
                self.status_thread = GetStatusThread(self.client_server, self.system_states)
                self.status_thread.status_signal.connect(self.add_system_status)
                self.status_thread.start()
            except Exception as e:
                self.add_system_status(str(e))
        else:
            self.add_system_status("Connection Unsuccessful. Try verifying the IP address.")
            self.server = None

    def receive_instructions(self):
        """
        will run in a receiver thread and will constantly check for updates from the server
        :return: Nothing
        """

        while True:
            
            if ser.in_waiting:
                self.ctrl.read_from_serail()
                
            try:
                self.client.receive_states()  # will hang up on this line until instructions are received
            except client.NoConnection:
                self.fail_state('No Connection')
                return

            while self.client.feedback_queue.qsize() > 0:  # if there are instructions in the queue
                token = self.client.feedback_queue.get(True, 3)
                param = token[0] # parse the instruction in terms of parameter and state
                state = token[1]

                if param == "connected" and state == "False": # checks if connection is ending
                    self.fail_state('Connection Ended')
                    return

                self.ctrl.write_to_serial(f'{param} {state}')  # send the command to our controller


class Controller:
    def __init__(self):
        pass