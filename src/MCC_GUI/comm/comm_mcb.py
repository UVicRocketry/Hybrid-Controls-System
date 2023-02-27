# UVR Propulsion Development Platform - MCB Communications Runtime
whoami = "MCC"
import serial
import queue
import betterconfigs
class connection():
    def __init__(self, device):
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
        try:
            self.stream = serial.Serial(port=self.port, baudrate=115200, timeout=0.1)
        except:
            pass
        self.status="DCONN"
        self.valves=["N2OF", "N2OV", "N2F", "RTV", "NCV", "EVV", "IGPRIME", "IGFIRE", "MEV"]
    def dummyData(self, data):
        self.message_queue.put(data)
    def doAbort(self):
        with self.message_queue.mutex:
            self.message_queue.queue.clear()
        self.send("MCC,ABORT")

    def send(self, message):
        print("Sending "+message)
        try:
            self.stream.write(message.encode())
            return 0
        except:
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
                return 1 #incorrect device type, return error
            elif mArr[1]=="SWITCHSTATE":
                print("MCB Set Switch State: "+mArr[2]+mArr[3])
                self.conf[mArr[2]]=mArr[3]
                self.control_queue.put([mArr[2],mArr[3]])
                return 1
            elif mArr[1]=="SUMMARY":
                print("Recieved MCB Switch Summary")
                for i in range(2,11):
                    self.conf[self.valves[i-2]]=mArr[i]
            elif mArr[1]=="STATUS":
                if mArr[2]=="STARTUP":
                    self.connected=False
                    print("Initializing connection to MCB...")
                    self.initConnection()
                elif mArr[2]=="DISARMED":
                    print("MCB Disarmed!")
                    self.status="DISARMED"
                elif mArr[2]=="ARMED":
                    print("MCB Armed!")
                    self.status="ARMED"
                elif mArr[2]=="ABORTED":
                    print("Aborted!")
                    self.status="ABORTED"
            elif mArr[1]=="PING":
                print("PONG!")
            elif mArr[1]=="ERROR":
                if mArr[2]=="UNKNOWNCOMMAND":
                    print(self.device+" Reported an Unknown Command")
            else:
                print("Unknown Message: "+message)
        else:
            return message
        
    def initConnection(self):
            while True:
                self.send("MCC,CONNECT")
                with self.message_queue.mutex:
                    self.message_queue.queue.clear()
                try:
                    confmsg = self.message_queue.get(timeout=3)
                    if confmsg=="MCB,STATUS,ESTABLISH":
                        print("Connection Established.")
                        self.status="CONN"
                        self.connected=True
                    break
                except queue.Empty:
                    print("Unable to connect, no response from device.")
                    break
                
                    
    def close(self):
        self.stream.close()
        self.status="DCONN"
        self.connected = False
