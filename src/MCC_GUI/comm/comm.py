# UVR Propulsion Development Platform - Serial Communications Runtime
whoami = "MCC"
import serial
import queue
import betterconfigs
import support.ezlogs
class connection():
    def __init__(self, device):
        self.log=support.ezlogs.LogFile(device+'.log')
        self.conf = betterconfigs.config(device+".save")
        self.message_queue = queue.Queue()
        self.control_queue = queue.Queue()
        try:
            self.port = self.conf["port"]
        except:
            self.port = "" #set blank port if no config file
        self.device = device
        self.connected = False
        self.desyncList=[]
        self.status="DCONN"
        self.valves=["N2OF", "N2OV", "N2F", "RTV", "NCV", "EVV", "IGPRIME", "IGFIRE", "MEV"]
        self.init_stream()
    def init_stream(self):
        try:
            self.port=self.conf["port"]
            self.stream = serial.Serial(port=self.port, baudrate=115200, timeout=0.1)
            self.log.log("INFO","Using port "+self.port)
        except:
            self.log.log("WARN", "Unable to connect to port "+self.port)
            pass
    def dummyData(self, data):
        self.message_queue.put(data)
        self.log.log("INFO","Dummy Data In: "+data)
    def doAbort(self):
        with self.message_queue.mutex:
            self.message_queue.queue.clear()
        self.send("MCC,ABORT")
        self.log.log("WARN","Abort Signal Triggered!")
    def send(self, message):
        try:
            self.stream.write(message.encode())
            self.log.log("INFO", "Sent: "+message)
            return 0
        except:
            self.log.log("WARN", "Tried to send: "+message)
            return 1
    def recieve(self):#this process should NEVER be called without being in a seperate thread, it will cause hanging
        message = ""
        try:
            message=message+self.stream.readline().decode()
            if message.endswith("\n"):
                message = message.strip()
                self.message_queue.put(message)
            if "ABORT" in message:
                with self.message_queue.mutex:
                        self.message_queue.queue.clear()
                self.message_queue.put(message)
        except:
            return 1
    def processCommand(self,message):
        if "," in message:
            mArr = message.split(",")
            if mArr[0]!=self.device:
                self.status="DEVERR"
                self.log.log("CRITICAL", "Recieved data for wrong device: "+message)
                return 1 #incorrect device type, return error
            elif mArr[1]=="SWITCHSTATE":
                self.log.log("INFO","Set Switch State: "+mArr[2]+mArr[3])
                self.conf[mArr[2]]=mArr[3]
                if self.device=="MCB":
                    self.control_queue.put([mArr[2],mArr[3]])
                return 1
            elif mArr[1]=="SUMMARY":
                self.log.log("INFO","Recieved Switch Summary")
                for i in range(2,11):
                    self.conf[self.valves[i-2]]=mArr[i]
            elif mArr[1]=="STATUS":
                if mArr[2]=="STARTUP":
                    self.connected=False
                    self.log.log("INFO","Recieved startup signal, initializing connection.")
                    self.initConnection()
                elif mArr[2]=="DISARMED":
                    self.log.log("INFO","Recieved Disarm Signal")
                    self.status="DISARMED"
                elif mArr[2]=="ARMED":
                    self.log.log("WARN","Recieved Disarm Signal")
                    self.status="ARMED"
                elif mArr[2]=="ABORTED":
                    self.log.log("WARN","Recieved Abort Signal")
                    self.status="ABORTED"
            elif mArr[1]=="PING":
                self.log.log("INFO", "Recieved ping")
                print("PONG!")
            elif mArr[1]=="ERROR":
                if mArr[2]=="UNKNOWNCOMMAND":
                    self.log.log("ERROR", "Device reported unknown command")
            else:
                self.log.log("ERROR", "Recieved unknown message: "+message)
        else:
            return message
        
    def initConnection(self):
            while True:
                self.send("MCC,CONNECT")
                with self.message_queue.mutex:
                    self.message_queue.queue.clear()
                try:
                    confmsg = self.message_queue.get(timeout=3)
                    if confmsg==(self.device+",STATUS,ESTABLISH"):
                        self.log.log("INFO", "Connection Established")
                        self.status="CONN"
                        self.connected=True
                    break
                except queue.Empty:
                    self.log.log("ERROR", "Unable to connect to device, no response")
                    break
                
                    
    def close(self):
        self.stream.close()
        self.status="DCONN"
        self.connected = False
        self.log.log("INFO", "Closed connection")
    def getLog(self):
        try:
            with open(self.device+".log", "r") as mcblog:
                return mcblog.read()
        except:
            return "No Log Found."
