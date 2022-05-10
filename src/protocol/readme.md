# Controls Protocol
This protocol is for the purpose of sending control messages between the control box, valve cart, and associated infrastructure of the PDP. Both serial and ethernet communication are used in the system. Since the data sent through these networks is very similar, they share the same communication protocal.

## Standard Protocal

All data should take the STANDARD FORMAT: ID,TAG,label,value, ....,TERMINATOR
For example: `"CBX,CD,stepper1,OPEN,stepper2,CLOSE,\n"`

The detailed description of each entry type are below:

- ID: a identifier for the message being sent which gives information about the sender.
  - CBX (Control Box)
  - MCC (Mission Control Laptop)
  - RPI (R-Pi Server)
  - VCA (Valuve Control Arduino)  

- TAG: Specifies type of message
  - CD (control data)
  - RQ (request)
  - ER (error message)
  - FD (feed back data)

- label: specifies type of control data, request, error ect.

- value: Value corresponding to the immediatly PRECEDING label.
  
- TERMINATOR: Indicates the end of a message packet currently set to : \n