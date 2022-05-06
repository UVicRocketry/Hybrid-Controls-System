# Controls Protocol
This protocol is for the purpose of sending control messages between the control box, valve cart, and associated infrastructure of the PDP. The protocol describes messages carrying valve state instructions, status, abort codes, and message acknowledgments.

## General
- Size: 4 bytes (32 bits)
- Byteorder: big endian

## Tags
The TAG is a three bit field indicating the message type. The following table describes the existing modes.

| Tag   | Code (hex) | Description                                                                                                                                                              |
| ----- | ---------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| SSI   | 0x0        | (System State Instruction) Control box status to instruct actuators. Initiated from the control box, then relayed through the MCC and R-Pi to the valve control Arduino. |
| SSS   | 0x1        | (System State Status) Relays actual system state to the MCC for logging and control box for LED updates.                                                                 |
| ABORT | 0x2        | Emergency abort message.                                                                                                                                                  |
| ACK   | 0x7        | Acknowledgment message.                                                                                                                                                                         |

## Fields
Following are field bit definitions.

### General 
| Field | Bit Range | Size (bits) | Description                                         |
| ----- | --------- | ----------- | --------------------------------------------------- |
| ID    | [0,7]     | 8           | Message identifier (unique except for ACK messages) |
| TAG   | [8,10]    | 3           | Message type                                        |

### SSI/SSS
| Field    | Bit Range | Size (bits) | Description                          |
| -------- | --------- | ----------- | ------------------------------------ |
| IGNITER  | [11]      | 1           | Igniter button status/Igniter status |
| VALVE_15 | [16]      | 1           | Switch status/Valve status           |
| VALVE_14 | [17]      | 1           | Switch status/Valve status           |
| VALVE_13 | [18]      | 1           | Switch status/Valve status           |
| VALVE_12 | [19]      | 1           | Switch status/Valve status           |
| VALVE_11 | [20]      | 1           | Switch status/Valve status           |
| VALVE_10 | [21]      | 1           | Switch status/Valve status           |
| VALVE_9  | [22]      | 1           | Switch status/Valve status           |
| VALVE_8  | [23]      | 1           | Switch status/Valve status           |
| VALVE_7  | [24]      | 1           | Switch status/Valve status           |
| VALVE_6  | [25]      | 1           | Switch status/Valve status           |
| VALVE_5  | [26]      | 1           | Switch status/Valve status           |
| VALVE_4  | [27]      | 1           | Switch status/Valve status           |
| VALVE_3  | [28]      | 1           | Switch status/Valve status           |
| VALVE_2  | [29]      | 1           | Switch status/Valve status           |
| VALVE_1  | [30]      | 1           | Switch status/Valve status           |
| VALVE_0  | [31]      | 1           | Switch status/Valve status           |