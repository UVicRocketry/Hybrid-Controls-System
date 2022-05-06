import enum

BYTEORDER = 'big'

class Enum(enum.Enum):
    # Field locations
    ID = 0xff000000
    TAG = 0x00e00000
    IGNITER = 0x00100000
    # Valves
    VALVES = 0x0000ffff
    VALVE_0 = 0x00008000
    VALVE_1 = 0x00004000
    VALVE_2 = 0x00002000
    VALVE_3 = 0x00001000
    VALVE_4 = 0x00000800
    VALVE_5 = 0x00000400
    VALVE_6 = 0x00000200
    VALVE_7 = 0x00000100
    VALVE_8 = 0x00000080
    VALVE_9 = 0x00000040
    VALVE_10 = 0x00000020
    VALVE_11 = 0x00000010
    VALVE_12 = 0x00000008
    VALVE_13 = 0x00000004
    VALVE_14 = 0x00000002
    VALVE_15 = 0x00000001

    # Field sizes
    MSG_SIZE = 32
    ID_SIZE = 8
    TAG_SIZE = 3
    VALVES_SIZE = 16

    # Tags
    ACK = 0x7
    SSI = 0x0
    SSS = 0x1
    ABORT = 0x2


class Message:

    def __init__(self, msg=None, id=None, tag=None, valves=(), igniter=False):
        if msg:
            if id or tag or valves or igniter:
                raise Exception("If msg is set other parameters cannot be used.")
            if type(msg) is bytes and len(msg) == Enum.MSG_SIZE/8:
                self.msg = int.from_bytes(msg, BYTEORDER)
            else:
                self.__check_input(msg, "msg", Enum.MSG_SIZE)
                self.msg = msg
        else:
            self.msg = 0x0
            if id:
                self.__check_input(id, "id", Enum.ID_SIZE)
                self.msg |= id << 24
            if tag:
                self.__check_input(tag, "tag", Enum.TAG_SIZE)
                self.msg |= tag << 21
            if valves:
                if type(valves) is tuple:
                    for i in range(len(valves)):
                        self.msg |= 0x1 << i
                else:
                    self.__check_input(valves, "valves", Enum.VALVES_SIZE)
                    self.msg |= valves
            if igniter:
                self.msg |= Enum.IGNITER
    
    def __check_input(input, varname, bits):
        if type(input) is not int:
            raise TypeError(varname, "should be of type int.")
        if input > 2**bits or input < 0:
            raise ValueError(varname, "should be non-negative and", bits, "bit(s).")

    def from_bytes(self, bytes):
        self.msg = int.from_bytes(bytes, BYTEORDER)

    def to_bytes(self):
        return self.msg.to_bytes(Enum.MSG_SIZE/8, BYTEORDER)

    def id(self):
        return (self.msg & Enum.ID) >> 24

    def setId(self, id):
        self.__check_input(id, "id", Enum.ID_SIZE)
        self.msg &= ~Enum.ID
        self.msg |= id << 24

    def tag(self):
        return (self.msg & Enum.TAG) >> 21

    def setTag(self, tag):
        self.__check_input(tag, "tag", Enum.TAG_SIZE)
        self.msg &= ~Enum.TAG
        self.msg |= tag << 21

    # Throws error if tag is not 0b000
    # Returns boolean value (0 if false, non-zero if true)
    def valve(self, valve_number):
        if self.tag() != Enum.SS:
            raise KeyError("Valve states may not be accurate for this tag.")
        return  self.msg & 0x1 << valve_number

    def set_valve(self, valve_number, on=True):
        if on:
            self.msg |= 0x1 << valve_number
        else:
            self.msg &= (0x0 << valve_number)

    def valves(self):
        return (self.msg & Enum.VALVES)

    def set_valves(self, valves):
        self.__check_input(valves, "valves", Enum.VALVES_SIZE)
        self.msg &= ~Enum.VALVES
        self.msg |= valves

    # Returns boolean value (0 if false, non-zero (specifically ) if true)
    def igniter(self):
        return self.msg & Enum.IGNITER

    def set_igniter(self, on):
        if on:
            self.msg |= Enum.IGNITER
        else:
            self.msg &= ~Enum.IGNITER