# UVR Propulsion Development Platform - Message helper class
# Owen Crewe 2022

# This class is intended to represent the defined message protocol.

# TODO:
# - Handle abort and igniter messages

class InvalidMsg(Exception):
    
    def __init__(self, msg="Message parsing failed. Message may have been disrupted in transmission."):
        super().__init__(msg)

VALID_ID = ( \
    "CBX", \
    "MCC", \
    "RPI", \
    "VCA", \
)

VALID_TAG = ( \
    "CD", \
    "RQ", \
    "ER", \
    "FD", \
)

VALID_STATE = ( \
    "OPEN", \
    "CLOSE", \
)

TERMINATOR = '\r\n'
DELIMITER = ','

class Message:

    def __init__(self, msg=None):
        self.data = dict()

        if msg:
            # If msg doesn't have id, tag, and terminator, raise error
            if len(msg.split(DELIMITER)) < 3:
                raise InvalidMsg("case 1")

            if msg.split(DELIMITER)[-1] != TERMINATOR:
                raise InvalidMsg("case 2")

            # Parse id and tag
            (self.id, self.tag, msg) = msg.split(DELIMITER, 2)

            # Raise error if ID or TAG aren't recognized
            if self.id not in VALID_ID:
                raise InvalidMsg("Invalid ID: ", self.id)
            if self.tag not in VALID_TAG:
                raise InvalidMsg("Invalid TAG: ", self.tag)
            
            # Parse label-value pairs
            msg = msg.split(DELIMITER)[:-1]
            if len(msg)%2 != 0:         # Raise error if odd number of entries in label-value section.
                raise InvalidMsg("Invalid number of entries in message.\nFormat: ID,TAG,[label,value]*,TERMINATOR")
            for i in range(0, len(msg), 2):
                if msg[i+1] not in VALID_STATE:
                    raise InvalidMsg("Invalid state: "+ msg[i]+ ":"+ msg[i+1])
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

    def __repr__(self):
        if self.id and self.tag:
            return self.id+DELIMITER+self.tag+DELIMITER+DELIMITER.join(label+DELIMITER+str(value) for (label,value) in self.data.items())+DELIMITER+TERMINATOR
        else:
            raise InvalidMsg("Cannot print an invalid message.")

    def __str__(self):
        return self.__repr__()