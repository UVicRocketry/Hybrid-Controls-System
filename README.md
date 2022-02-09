# Hybrid_Controls_System (HB - Project Acronym)

The controls software works on a server-client model. The server role is filled by mission control where the GUI (found in hybrid_test_backend.py) communicates with the control module on the test stand. The client role is filled by a raspberry pi running the control software (found in controller.py) which “talks” to an arduino through a USB port. 

The client and server send commands to each other in order to control the test stand. The commands take the form of a parameter and a state separated by a space. For example, to close the MEV, the server would send the following message: “MEV closed”. The client would receive that command and update the arduino accordingly. After receiving confirmation that the MEV had been closed successfully, the client would send a confirmation message back to the server which would take the same form as the original command e.g. “MEV closed”

## Example Control Flow

Server sends “MEV closed” to the client
Client sends “MEV closed” to the arduino
Arduino sends “MEV closed” to the client after the MEV closes successfully
The client sends “MEV closed” back to the server so the server knows that the command executed successfully

## Objectives
control all feed system valves through stepper motors, pneumatic actuators, and limit switches
House motor drivers and 
Constraints:
Must allow control of valves from a safe distance from test stand

## Implementation (How to Connection)

The hybrid_test_backend.py file imports a Server object from the server.py file. Make sure that both of those files are present in the same folder when running the software

The controller.py file imports a Client object from the client.py file. Make sure that both of those files are present in the same folder when running the software.

At the top of the controller.py file are two values HOST and PORT. 
The HOST value is the IP of the computer that is running the server (hybrid_test_backend.py)

To determine the correct IP:
1. On the computer that is running hybrid_test_backend.py open command prompt
2. Type in ipconfig and hit enter
3. Find line labelled IPv4 Address. That is the correct IP address
![IPconnection](./source/images/IPconnection.png)

You should make sure that the HOST value in controller.py matches the value of the IPv4 address before you attempt to run the software

There is a PORT value in both hybrid_test_backend.py and in controller.py, make sure they are the same before attempting to run the software

### Common Issues
If you are having trouble getting the connection to establish:
Double check that the IPv4 address matches HOST
Double check that the two PORT values match

Your computer’s firewall will likely block the connection initially. If your connection is failing and none of the above fixes work try the following steps.

1. Disable the firewall on both devices (the raspberry pi likely doesn’t have a firewall so don’t worry about that) 
2. Attempt the connection again
3. If it works this time and you don’t want to permanently leave your firewall off follow the steps in the following tutorial to open your firewall to that port https://www.tomshardware.com/news/how-to-open-firewall-ports-in-windows-10,36451.html

## Testing

To start testing the system you first must wired the everything correctly according to this diagram.  

![Wiring Diagram](./source/images/ControlsWiringDiagram.drawio.png)
_Note: If you get the draw.io extension for vs code you can edit this picture directly._

  Next plugin the computer running the GUI to the Rpi 4 using a ethernet cable. You can now start up the server and connect to the client. To do this you have to run controller.py on the raspberry pi and hybrid_test_backend.py on the GUI computer. Once both of those are running you can follow the connection steps in implementation.

  To test make sure the arduino folder is uploaded to the arduino and the QueueList is working imported properly. To connect to the arduino you need to change `serial_port` in controller.py to the correct port on the raspberry Pi.

   
  
I will finish this when im less tired ~ Kris