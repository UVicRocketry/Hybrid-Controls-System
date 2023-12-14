# Hybrid-Controls-System (HB - Project Acronym)

**This project is in a release cycle, please submit bug reports to the Issues tab**

## Project Information

UVic Rocketry's hybrid controls system is designed to actuate and monitor the states of stepper motor driven valves located on a high-pressure oxidizer feed system. The controls system includes the following components:
- Mission Control Board (MCB) : A physical panel with toggle switches, momentary buttons, indicator lights, and a buzzer
  - Sends commands to Mission Control Computer based on user input through switches/buttons
- Mission Control Computer (MCC) : The main controller and communication method with the valve cart
  - Manages commands from the MCB and forwards them to the valve cart 
  - Confirms valve status from the valve cart
  - Also able to be used as an override in case the MCB stops working
- Valve Control Cart (VC) : Actuates all valves on the test stand
  - Recieves command from Mission Control Computer
  - Executes valve movements
  - Obtains confirmation or error from limit switches
  - sends confirmed valve states or error back to the Mission Control Computer

![System_Control_Flow_Diagram](./images/high_level_data_flow.drawio.png)  
_Note: If you get the draw.io extension for vs code you can edit this picture directly._
**Possibly out of date and needs review!**


## Installation and Setup of the GUI
The GUI for the Mission Control Computer is the brains of both the Valve Cart and the Mission Control Board, as well as the communication between them. 

There are two options for installing the GUI and communication libraries: downloading a released compiled version, or downloading directly from source and running as a python script.

### Downloading and Running a Release
Releases of the GUI are bundled into a single file using PyInstaller, and built for x86_64 architecture **only**. If you need an x86 version, refer to the compiling from source instructions of this document.

1. To find the latest release, go to <https://github.com/UVicRocketry/Hybrid-Controls-System/releases> and download the executable for your system.

2. Once downloaded, you may need to enable execution of the executable (refer to your system's documentation). On Linux, this is `chmod +x HybridControlsGUI`

3. Finally, just run the GUI by either double-clicking or executing from the terminal.

### Downloading and Running from Source
Running from source is necessary if you want to be bleeding edge, or if there's no release for your system. This method is bit finicky, but if you follow the steps below, you'll be good to go

1. **Make sure you have the right Python version**, you must have Python 3.9 or above for the libraries you need to work.
2. **Install pre-requisites**: Use the following command to install all the pre-requisites you need for the GUI: `pip3 install betterconfigs pyserial PyQt5`
3. **Download from Source**: You can pull the bleeding commit from the git repo using `git pull https://github.com/UVicRocketry/Hybrid-Controls-System.git`
4. **Checkout the right branch**: Make sure you're in the right branch by cd-ing into the git repo you just downloaded and using `git checkout *branchname*` (replace branchname with the right branch name)
5. **Run the GUI**: If you've done everything right, you can run the GUI by cd'ing into `./src/MCC_GUI/` and running `python3 gui_valve.py`. If you get an error, refer to the *Common Errors* section below

## Command Structure Examples
Command structure breakdown:

VC,Command,Modifier,Modifier,etc

EX:

VC,CTRL,MEV,OPEN - opens MEV valve

VC,CTRL,N2OF,CLOSE - closes N2OF valve

VC,STATUS - reports status of all valves

VC,ABORT - initiates abort seequence


## Common Issues & Errors
**If you can open the GUI, and you're having issues with communication, use the debug console to help troubleshoot**
<details><summary>The GUI opened, but I can't see any serial devices to select</summary>
1. Is the device plugged in and receiving power? (It's okay I do it all the time too). Also, try hitting the refresh button.<br>
2. If you're getting power to the device, check the OS's device manager and see if it's showing up. If it is, you might have a permissions error with the applications. Linux especially has this issue (Your user might need to be added to the dialout group).<br>
</details>

<details><summary>I'm not able to run the GUI</summary>
Try running in your terminal and seeing if it gives you any errors. Otherwise, your window manager might not be able to run Qt applications (looking at you GTK+!)
</details>

<details><summary>Pip won't let me install the library betterconfigs</summary>
I've seen this error before, but not really sure why it happens even though the Python version is correct. You can go to PyPi and install manually if it's giving you trouble.
</details>

<details><summary>I'm not sure which branch to pull from</summary>
Ask the maintainers! If you've got access to the Discord, just message @Propulsion and someone should be able to help you out. If nobody knows, probably the latest push is the best.
</details>

<details><summary>It's still broken</summary>
If nobody in propulsion can help, contact <a href="mailto:iainrosen@uvic.ca">iainrosen@uvic.ca</a> and he should be able to help you out!
</details>
