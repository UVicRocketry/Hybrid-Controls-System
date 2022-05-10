class InvalidMsg(Exception):
    
    def __init__(self, msg="Message parsing failed. Message may have been disrupted in transmission."):
        super().__init__(self.msg)

class Message:

    def __init__(self, msg=None):
        self.data = dict()

        if msg:
            if len(msg.split(',')) < 2 or msg.split(',')[-1] != '\n':
                raise InvalidMsg()
            (self.id, self.tag, msg) = msg.split(',', 2)
            msg = msg.split(',')[:-1]
            if len(msg)%2 != 0:
                raise InvalidMsg()
            for i in range(len(msg)/2):
                self.data[msg[i]] =  msg[i+1]
        else:
            self.id = None
            self.tag = None

    def getId(self):
        return self.id

    def setId(self, id):
        self.id = id

    def getTag(self):
        return self.tag

    def setTag(self, tag):
        self.tag = tag

    def getLabels(self):
        return self.data.keys()

    def getValue(self, label):
        return self.data[label]

    def setValue(self, label, value):
        self.data[label] = value