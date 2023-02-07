# UVR Propulsion Development Platform - Valve Cart Control Graphical User Interface

#Iain Rosen 2023

#make sure to compile the ui file to py using: python3 -m PyQt5.uic.pyuic test.ui -o test.py
import sys
import os
from PyQt5.QtWidgets import *
from qt.main import Ui_MainWindow
import comm.comm_vc
import threading
import time
from datetime import datetime
from qt.VCPort import Ui_Dialog

def thread_recieve():
    while True:
        vc.recieve()
def active_process():
    while True:
        vc.processCommand(vc.message_queue.get())
        window.ui.l_PINGDYN.setText(datetime.now().strftime("%H:%M:%S"))
def ping_vc():
    while True:
        if vc.connected==True:
            try:
               for i in vc.valves:
                    vc.send("MCC,GET,"+i)
                    time.sleep(1)
            except:
                pass
def check_status():
    while True:
        try:
            window.ui.l_STATUS.setText(vc.status)
            window.ui.l_N2OFlow.setText(vc.conf["N2OF"])
            window.ui.l_N2OValve.setText(vc.conf["N2OV"])
            window.ui.l_N2Flow.setText(vc.conf["N2F"])
            window.ui.l_RTV.setText(vc.conf["RTV"])
            window.ui.l_NCV.setText(vc.conf["NCV"])
            window.ui.l_EVV.setText(vc.conf["EVV"])
            window.ui.l_IGPRIME.setText(vc.conf["IGPRIME"])
            window.ui.l_IGFIRE.setText(vc.conf["IGFIRE"])
            window.ui.l_MEV.setText(vc.conf["MEV"])
        except:
            pass
        time.sleep(0.5)
def flip_switch(switchID):
    try:
        if vc.conf[switchID]=="OPEN":
            vc.send("MCC,CTRL,"+switchID+",CLOSE")
        elif vc.conf[switchID]=="CLOSE":
            vc.send("MCC,CTRL,"+switchID+",OPEN")
        else:
            vc.send("MCC,CTRL,"+switchID+",OPEN")
    except:
        pass
def close_and_exit():
    vc.close()
    os._exit(0)
class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        #triggers

        self.ui.actionPerform_Reconnect.triggered.connect(vc.initConnection)
        self.ui.b_ABORT.clicked.connect(vc.doAbort)
        self.ui.b_CLOSE.clicked.connect(close_and_exit)
        self.ui.b_N2OFlow.clicked.connect(lambda: flip_switch("N2OF"))
        self.ui.b_N2OValve.clicked.connect(lambda: flip_switch("N2OV"))
        self.ui.b_N2Flow.clicked.connect(lambda: flip_switch("N2F"))
        self.ui.b_RTV.clicked.connect(lambda: flip_switch("RTV"))
        self.ui.b_NCV.clicked.connect(lambda: flip_switch("NCV"))
        self.ui.b_EVV.clicked.connect(lambda: flip_switch("EVV"))
        self.ui.b_IGPRIME.clicked.connect(lambda: flip_switch("IGPRIME"))
        self.ui.b_IGFIRE.clicked.connect(lambda: flip_switch("IGFIRE"))
        self.ui.b_MEV.clicked.connect(lambda: flip_switch("MEV"))

if __name__ == '__main__':
    #connect to port
    vc = comm.comm_vc.connection(port="", device="VC")
    app = QApplication([])
    try:
        vc.stream()
    except:
        VCPort=QDialog()
        VCPort.ui = Ui_Dialog()
        VCPort.ui.setupUi(VCPort)
        try:
            VCPort.ui.t_VCPort.setText(comm.comm_vc.betterconfigs.config("VC.save")["port"])
        except:
            pass
        VCPort.exec()
        vc=comm.comm_vc.connection(VCPort.ui.t_VCPort.text(), "VC")
        
    window = MainWindow()
    window.show()
    
    print(vc.port)
    p=threading.Thread(target=thread_recieve)
    p.start()
    s=threading.Thread(target=check_status)
    s.start()
    t=threading.Thread(target=active_process)
    t.start()
    z=threading.Thread(target=ping_vc)
    z.start()
    sys.exit(app.exec_())
