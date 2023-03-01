from datetime import datetime

class LogFile():
    def __init__(self, filename):
        self.filename = filename
        with open(self.filename, "a") as fileIO:
            fileIO.write("["+timenow()+"] INFO: Start of LogFile "+self.filename+"\n")
    def log(self, type, logMessage):
        with open(self.filename, "a") as fileIO:
            fileIO.write("["+timenow()+"] "+type+": "+logMessage+"\n")
def timenow():
    return datetime.now().strftime("%H:%M:%S")