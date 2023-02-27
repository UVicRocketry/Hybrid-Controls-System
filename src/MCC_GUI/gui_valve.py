#UVR Propulsion Development Platform - Valve Cart Control Graphical User Interface

#Iain Rosen 2023

#make sure to compile the ui file to py using: compileGUI.sh
import sys
import os
from PyQt5.QtWidgets import *
from qt.main import Ui_MainWindow
import comm.comm_vc
import comm.comm_mcb
import threading
import time
from datetime import datetime
import qt.PortSel
import qt.confirm
def connection_init(vcPort, mcbPort):
    vc.port=vcPort
    mcb.port=mcbPort
    vc.conf["port"]=vcPort
    mcb.conf["port"]=mcbPort
def vc_recieve():
    while True:
        vc.recieve()
def mcb_recieve():
    while True:
        mcb.recieve()
def vc_active_process():
        while True:
            vc.processCommand(vc.message_queue.get())
            window.ui.l_PINGDYN.setText(datetime.now().strftime("%H:%M:%S"))
def mcb_active_process():
    while True:
        mcb.processCommand(mcb.message_queue.get())
def control_queue_process():
    while True:
        ctrlcmd = mcb.control_queue.get()
        if ctrlcmd[0] not in mcb.desyncList:
            vc.send("MCC,CTRL,"+ctrlcmd[0]+","+ctrlcmd[1])
def ping_vc():
    while vc.connected:
        try:
            vc.send("MCC,SUMMARY")
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
        try:
            mcb.desyncList=[]
            for i in mcb.valves:
                if mcb.conf[i]!=vc.conf[i]:
                    mcb.desyncList.append(i)
                    
            window.ui.l_dyn_desync.setText(str(mcb.desyncList))
        except:
            pass
        time.sleep(0.5)
def flip_switch(switchID):
    try:
    #get the desired state
        if vc.conf[switchID]=="OPEN":
            dState = "CLOSE"
        elif vc.conf[switchID]=="CLOSE":
            dState = "OPEN"
        else:
            dState = "OPEN"
    except:
        dState="OPEN"
    #confirm the action
    confirm.ui.l_actionList.setText(switchID+" "+dState)
    confirm.ui.c_confirm.setCheckState(False)
    confirm.exec()

    if not confirm.ui.c_confirm.isChecked():
        return
    else:
        try:
            vc.send("MCC,CTRL,"+switchID+","+dState)
        except:
            pass
def close_and_exit():
    vc.close()
    mcb.close()
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
        self.ui.actionConnection_Selector.triggered.connect(lambda: portsel.exec())
class PortSelector(QDialog):
    def __init__(self):
        super(PortSelector, self).__init__()
        self.ui = qt.PortSel.Ui_Dialog()
        self.ui.setupUi(self)
        #prefill
        self.ui.t_VCPort.setText(vc.port)
        self.ui.t_MCBPort.setText(mcb.port)
        #triggers
        self.ui.b_VCPort.clicked.connect(lambda: connection_init(vcPort=self.ui.t_VCPort.text(), mcbPort=self.ui.t_MCBPort.text()))
class ConfirmDiag(QDialog):
    def __init__(self):
        super(ConfirmDiag, self).__init__()
        self.ui = qt.confirm.Ui_Dialog()
        self.ui.setupUi(self)
        
if __name__ == '__main__':
    #setup connections with blank params
    vc = comm.comm_vc.connection(device="VC")
    mcb = comm.comm_mcb.connection(device="MCB")
    #setup QApplication
    app = QApplication([])
    window = MainWindow()
    portsel = PortSelector()
    confirm = ConfirmDiag()
    portsel.exec()
    window.show()
    threading.Thread(target=vc_recieve).start()
    threading.Thread(target=mcb_recieve).start()
    threading.Thread(target=check_status).start()
    threading.Thread(target=vc_active_process).start()
    threading.Thread(target=mcb_active_process).start()
    threading.Thread(target=ping_vc).start()
    threading.Thread(target=control_queue_process).start()
    sys.exit(app.exec_())
