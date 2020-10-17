"""
This file takes input from the engine stepper motors and other elements
from the PCB and PI and decodes the data into information that the GUI back end
can read and send it through client.py. 
"""
#Throwing strings as communication protocol 
#
#assume everything is a dictionary
# 

import client 

class EncryptEngine:
    """
    This class gets the information from the engine or client and
    encrypts it in a way to send to send to the client.
    """
    __init__(self):
        self.server = client.Client(999,999)#connect client to get information from the gui

        # client._initialize_connection() 

    #instruction data frame
    
    def decode_instructions(self):
        """
        takes the information from the socket   
        """
    def connect(self):
        
        # if ():

class RunStepper:
    """
    This class will do all the operations of moving a stepper motor
    """
    __init__(self):


    




