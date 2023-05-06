#UVR Propulsion Development Platform - Valve Cart Control Graphical User Interface

#Iain Rosen 2023

#make sure to compile the ui file to py using: compileGUI.sh
import sys
import os
from PyQt5.QtWidgets import *
from qt.main import Ui_MainWindow
import commlib.comm as comm
import threading
import time
import serial.tools.list_ports
from datetime import datetime
import qt.PortSel
import qt.confirm
import qt.debug

#multi-threading processes
def thread_recieve():
    while True:
        if vc.messagePending():
            vc.recieve()
        if mcb.messagePending():
            mcb.recieve()
def thread_active_process():
        while True:
            if vc.message_queue.qsize() != 0:
                vc.processCommand(vc.message_queue.get())
                window.ui.l_PINGDYN.setText(datetime.now().strftime("%H:%M:%S"))
            if mcb.message_queue.qsize() != 0:
                mcb.processCommand(mcb.message_queue.get())
                window.ui.l_PINGDYN.setText(datetime.now().strftime("%H:%M:%S"))
            time.sleep(0.3)
def thread_ping_status():
    while True:
        try:
            if vc.connected:
                vc.send("MCC,SUMMARY")
        except:
            pass
        try:
            window.ui.l_STATUS.setText(vc.status)
            window.ui.l_dyn_status.setText(mcb.status)
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
        time.sleep(1)
def thread_control_queue_process():
    while True:
        ctrlcmd = mcb.control_queue.get()
        if ctrlcmd[0] not in mcb.desyncList:
            vc.send("MCC,CTRL,"+ctrlcmd[0]+","+ctrlcmd[1])

def change_port(vcPort, mcbPort):
    vc.conf["port"]=vcPort
    mcb.conf["port"]=mcbPort
    vc.init_stream()
    mcb.init_stream()
def flip_switch(switchID):
    try:
        #check if abort pressed
        if switchID=="ABORT":
            confirm.ui.l_actionList.setText("ABORT VALVE CART")
            confirm.ui.c_confirm.setCheckState(False)
            confirm.exec()
            if confirm.ui.c_confirm.isChecked():
                vc.doAbort()
                mcb.doAbort()
                return
            else:
                return
    except:
        pass
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
    vc.conf[switchID]="..."
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
        self.ui.b_ABORT.clicked.connect(lambda: flip_switch("ABORT"))
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
        self.ui.actionDebug_Console.triggered.connect(lambda: debugcon.show())
        self.ui.actionVC_Logs.triggered.connect(lambda: vc.clearLog())
        self.ui.actionMCB_Logs.triggered.connect(lambda: mcb.clearLog())
class PortSelector(QDialog):
    def __init__(self):
        super(PortSelector, self).__init__()
        self.ui = qt.PortSel.Ui_Dialog()
        self.ui.setupUi(self)
        #prefill
        self.ui.t_VCPort.setText(vc.port)
        self.ui.t_MCBPort.setText(mcb.port)
        self.ui.c_vc.addItem("")
        self.ui.c_mcb.addItem("")
        for port in serial.tools.list_ports.comports():
            self.ui.c_vc.addItem(port[0])
            self.ui.c_mcb.addItem(port[0])
        #triggers
        self.ui.b_VCPort.clicked.connect(lambda: change_port(vcPort=self.ui.t_VCPort.text(), mcbPort=self.ui.t_MCBPort.text()))
        self.ui.c_vc.currentIndexChanged.connect(lambda: self.ui.t_VCPort.setText(self.ui.c_vc.currentText()))
        self.ui.c_mcb.currentIndexChanged.connect(lambda: self.ui.t_MCBPort.setText(self.ui.c_mcb.currentText()))
        self.ui.b_refresh.clicked.connect(self.refreshConnections)
    def refreshConnections(self):
        self.ui.c_mcb.clear()
        self.ui.c_vc.clear()
        self.ui.c_vc.addItem("")
        self.ui.c_mcb.addItem("")
        for port in serial.tools.list_ports.comports():
            self.ui.c_vc.addItem(port[0])
            self.ui.c_mcb.addItem(port[0])
class ConfirmDiag(QDialog):
    def __init__(self):
        super(ConfirmDiag, self).__init__()
        self.ui = qt.confirm.Ui_Dialog()
        self.ui.setupUi(self)
class DebugConsole(QDialog):
    def __init__(self):
        super(DebugConsole, self).__init__()
        self.ui = qt.debug.Ui_Dialog()
        self.ui.setupUi(self)
        #triggers
        self.ui.pt_mcb.setPlainText(datetime.now().strftime("%H:%M:%S"))
        self.ui.pt_vc.setPlainText(datetime.now().strftime("%H:%M:%S"))
        self.ui.le_mcb.returnPressed.connect(lambda: mcb.dummyData(self.ui.le_mcb.text()))
        self.ui.le_mcb.returnPressed.connect(lambda: self.ui.le_mcb.setText(""))
        self.ui.le_vc.returnPressed.connect(lambda: vc.dummyData(self.ui.le_vc.text()))
        self.ui.le_vc.returnPressed.connect(lambda: self.ui.le_vc.setText(""))
        self.ui.c_vc_verbose.stateChanged.connect(lambda: vc.setVerbose(self.ui.c_vc_verbose.isChecked()))
        self.ui.c_mcb_verbose.stateChanged.connect(lambda: mcb.setVerbose(self.ui.c_mcb_verbose.isChecked()))
        #timed triggers
        self.timer = qt.debug.QtCore.QTimer()
        self.timer.timeout.connect(lambda: self.ui.pt_mcb.setPlainText(mcb.log.read()))
        self.timer.timeout.connect(lambda: self.ui.pt_mcb.verticalScrollBar().setValue(self.ui.pt_mcb.verticalScrollBar().maximum()))
        self.timer.timeout.connect(lambda: self.ui.pt_vc.setPlainText(vc.log.read()))
        self.timer.timeout.connect(lambda: self.ui.pt_vc.verticalScrollBar().setValue(self.ui.pt_vc.verticalScrollBar().maximum()))
        self.timer.start(1000)
if __name__ == '__main__':
    #setup connections with blank params
    vc = comm.connection(device="VC")
    mcb = comm.connection(device="MCB")
    #setup QApplication
    app = QApplication([])
    window = MainWindow()
    portsel = PortSelector()
    confirm = ConfirmDiag()
    debugcon = DebugConsole()
    portsel.exec()
    window.show()
    threading.Thread(target=thread_recieve).start()
    threading.Thread(target=thread_ping_status).start()
    threading.Thread(target=thread_active_process).start()
    threading.Thread(target=thread_control_queue_process).start()
    sys.exit(app.exec_())
