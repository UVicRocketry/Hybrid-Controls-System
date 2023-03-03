# Iain Rosen - hello@iainrosen.me
# Made this cause I hate PyLogging. Don't use this in your fancy projects it'll probably break.

from datetime import datetime,date

class LogFile():
    def __init__(self, filename):
        self.filename = filename
        self.log("INFO", "Start of Session")
    def log(self, type, logMessage):
        with open(self.filename, "a") as fileIO:
            fileIO.write(timenow()+" "+type+": "+logMessage+"\n")
    def read(self):
        with open(self.filename, 'r') as fileIO:
            return fileIO.read()
    def findByTime(self, time, date=str(date.today())):
        with open(self.filename, 'r') as fileIO:
            fL = fileIO.readlines()
        timeRough = time[0:5]
        closest="None"
        for i in fL:
            entryDate = i[0:10]
            entryTime = i[12:20]
            entryTimeRough = i[12:17]
            if entryDate==date and entryTime==time:
                return i
            elif entryDate==date and entryTimeRough==timeRough:
                closest=i
        if closest!="None":
            return closest
        else:
            return "Not Found."
def timenow():
    return str(date.today())+" ["+datetime.now().strftime("%H:%M:%S")+"]"