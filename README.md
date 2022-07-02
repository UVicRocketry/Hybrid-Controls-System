# Hybrid-Controls-System (HB - Project Acronym)

UVic Rocketry's hybrid controls system is designed to actuate and monitor the states of stepper motor driven valves located on a high-pressure oxidizer feed system. The controls system includes the following components:
- Controls Box : A physical panel with toggle switches, momentary buttons, indicator lights, and a buzzer
  - Sends commands to MC Computer based on user input through switches/buttons
  - turns on indicator light once valve state has been confirmed
- MC Computer : The "server" in the client-server communication model
  - Sends commands to R Pi based on controls box input 
  - Recieves valve state confirmation from R Pi
  - Sends valve state confirmation to Controls Box
- Raspberry Pi : The "client" in the client-server communication model
  - Recieves command from MC Computer
  - Relays command to Arduino
  - Recieves confirmation of valve state from arduino
  - Relays confirmation back to MC computer
  - Collects and Logs Data from LABJACK DAQ
  - Sends Logs to MC Computer
- Valve Control Arduino : Actuates all valves on the test stand
  - Recieves command from R Pi
  - Executes valve movements
  - Obtains confirmation or error from limit switches
  - sends confirmed valve states or error back to the R Pi

![System_Control_Flow_Diagram](./images/high_level_data_flow.drawio.png)  
_Note: If you get the draw.io extension for vs code you can edit this picture directly._

## Control Box Arduino
The Control Box Arduino is used for including physical switches, buttons, and safety measures that integrate with the virtual controls offered by the GUI software. The Arduino will be connected via USB to the Controls laptop and have the serial input interpreted by the GUI software. 

### Objectives
- maximize visibility
- maximize safety
- maximize response time
### Constraints
- e-stop press sends system to safe state
- physical control for arming system
- visual indicator for system armed
- visual and auditory indicator for system in auto mode
- max operating voltage is 5V
- max available GPIO dictated by Arduino

### Notes
- there are 8 available pins on the Nano, A2-A7, D11, D13
  - D11 was reading high when it should not have been, so should not be trusted with critical functions
  - D13 is wired to the Nano's LED and cannot be used for input unless the LED and resistor are desoldered
- there are 6 available outs on the shift registers left if more LEDs need to be added
- 4 valve switches are wired up but unused


## MCC
The Mission Control Laptop display data on the system to the user and relays communication from the Control Box to the R-PI.

### Objectives
- Display Valve states
- Display instrumentation data
### Constraints
- Relays Control Box data to the R-PI

## R-PI
The R-PI is the local connection for the Hybrid test stand. It records and logs the data from the LABJACK DAQ and sends this data to the MCC for display. More importantly, currently the R-PI relays data to the Valve Control Arduino from the MCC
### Objectives
- Sends data logs to MCC
- Sends valve data to MCC and Control Box
### Constraints
- Relays Control Box data to the Valve Control Arduino
- Logs data from instrumentation



## Valve Control Arduino

The Arduino in charge of valve control accepts commands from the R Pi over serial. The arduino is then in charge of rotating the stepper motors until limit switches are contacted, allowing the Arduino to send back a confirmation of the valve state. 

### Objectives
- provide mission control with accurate valve states as often as necessary
### Constraints
- execute commands given by R Pi
- confirm valve state 
- must initialize valves to safe state (ie. allows for short power loss or e-stop reset to put system into safe state)
- must identify stuck valves and relay to Mission Control (can use timer)
- Must initiate abort system when safety checks fail
### Wiring
![Wiring Diagram](./images/ControlsWiringDiagram.drawio.png)  
_Note: If you get the draw.io extension for vs code you can edit this picture directly._

## Communication Protocol
Both serial and ethernet communication are used in the system. Since the data sent through these networks is very similar, they share the same communication protocal.

### Standard Protocol

All data should take the STANDARD FORMAT:    ID,TAG,label,value, ....,TERMINATOR
For example: CBX,CD,stepper1,OPEN,stepper2,CLOSE,\n

The detailed description of each entry type are below:

- ID: a identifier for the message being sent which gives information about the sender.
  - Code (description),
  - CBX (Control Box)
  - MCC (Mission Control Laptop)
  - RPI (R-Pi Server)
  - VCA (Valuve Control Arduino)  

- TAG: Specifies type of message
  - Code (description),
  - CD (control data),
  - RQ (request),
  - ER (error message)
  - FD (feed back data)]
  - CF (Confirm message recieved "value")

- label: specifies type of control data, request, error ect.

- value: Value corresponding to the immediatly PRECEDING label.
  
- TERMINATOR: Indicates the end of a message packet currently set to : \n

### Error Protocol

- COMMUNICATION ERROR: A communication error occurs when one subsystems fails to get confirmation from another. When this occur, a two way ERROR message is sent to every available subsystem.

### Abort Protocol
- ABORT (high priority message): The ABORT message needs to bipass regular message handling and be relayed as quickly as possible to the Valve Control Arduino.

### Ignition Protocol
- IGNITE (priority message): The ignition sequence is a special state which requires a button to be held and a repeating stream of data to be sent through the communication system to the valve control arduino. When the stream of data stops (indicated by a timeout period) ignition ceases and regular control is resumed. This protocol allows for a continuous ignition process.

### Mission Control Laptop & Raspberry Pi Communications
The communication between Mission control and the Test Stand is facilitated by a python-written server-client model. The client and server send commands to each other in order to control the test stand. The commands take the form of a parameter and a state separated by a space. For example, to close the MEV, the server would send the following message: “MEV closed”. The client would receive that command and update the arduino accordingly. After receiving confirmation that the MEV had been closed successfully, the client would send a confirmation message back to the server which would take the same form as the original command e.g. “MEV closed”

#### Implementation (How to Establish Connection)

The hybrid_test_backend.py file imports a Server object from the server.py file. Make sure that both of those files are present in the same folder when running the software

The controller.py file imports a Client object from the client.py file. Make sure that both of those files are present in the same folder when running the software.

At the top of the controller.py file are two values HOST and PORT. 
The HOST value is the IP of the computer that is running the server (hybrid_test_backend.py)

To determine the correct IP:
1. On the computer that is running hybrid_test_backend.py open command prompt
2. Type in ipconfig and hit enter
3. Find line labelled IPv4 Address. That is the correct IP address
![IPconnection](./images/IPconnection.png)

You should make sure that the HOST value in controller.py matches the value of the IPv4 address before you attempt to run the software

There is a PORT value in both hybrid_test_backend.py and in controller.py, make sure they are the same before attempting to run the software

#### Common Issues
If you are having trouble getting the connection to establish:
Double check that the IPv4 address matches HOST
Double check that the two PORT values match

Your computer’s firewall will likely block the connection initially. If your connection is failing and none of the above fixes work try the following steps.

1. Disable the firewall on both devices (the raspberry pi likely doesn’t have a firewall so don’t worry about that) 
2. Attempt the connection again
3. If it works this time and you don’t want to permanently leave your firewall off follow the steps in the following tutorial to open your firewall to that port https://www.tomshardware.com/news/how-to-open-firewall-ports-in-windows-10,36451.html



## Testing and Requirements
![Wiring Diagram](./images/ControlsWiringDiagram.drawio.png)

See HCS_Requirements_Documentation.xlsx for specific Testing and Requirements information


