//import valve class and other needed classes 
#include <map>//might have to import onto ardunio
#include <string>
#include <queue>
//input valve header file HERE

using namespace std;

//declare global variable and all functions below
string data;
queue<string> commandsCompleted;
map<string, int> activePins; 
bool nextValue;

void flashOnce();
void flashTwice();
bool sendData();
void runCommands();
void setupPins();

 
void setup()
{
    setupPins();
    // pinMode(); must setup all pins
    Serial.begin(115200);
}

void loop()
{
    if (Serial.available())
    {
        data = Serial.readString();
        runCommands();
        nextValue = !commandsCompleted.isEmpty() && sendData();
    }
    if (nextValue) 
    {
        nextValue = sendData(); 
    }
    else 
    {
        nextValue = false;
    }
}

/*
    For flashing functions below (flashes on the arduino)
    First set of flashes indicates the target. Second set of flashes indicates setting.
    If there's only one set of flashes, it's the igniter.
    1 flash means setting closed, 2 flashes means setting open.
    First set:
        0 flashes - igniter
        1 flashes - MEV
        2 flashes - N2OV
        3 flashes - N2O
        4 flashes - N2
        5 flashes - NCV
        6 flashes - RV
        7 flashes - N2V
    Contact Kris L for more
*/
void flashOnce(int repeat)
{
    for(int i = 0; i < repeat; i++)
    {
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
        delay(500);
    }
}

void flashTwice(int repeat)
{
    for(int i = 0; i < repeat; i++)
    {
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
        delay(500);
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
        delay(500);
    }
}
/* 
    This function maps command string values to a number 
    value to for the switch statement in runcommands
*/
int dataToNumber()
{
    int numberCommand = (data == "MEV closed") ? 0 
            : (data == "MEV open") ? 1 
            : (data == "N2OV closed") ? 2 
            : (data == "N2OV open") ? 3 
            : (data == "N2O closed") ? 4 
            : (data == "N2O open") ? 5 
            : (data == "N2 closed") ? 6 
            : (data == "N2 open") ? 7 
            : (data == "NCV closed") ? 8 
            : (data == "NCV open") ? 9 
            : (data == "RV closed") ? 10 
            : (data == "RV open") ? 11 
            : (data == "N2V closed") ? 12 
            : (data == "N2V open") ? 13
            : NULL;//bad command set to null if this happens send error msg to pi
    return numberCommand;
}

bool sendData()
{
    try {
        Serial.println(commandsCompleted.pop());
        if (commandsCompleted.isEmpty())//data more caluses to handling 
        {
            throw(Serial.println("No command completed"));
        }
    }
    catch (...)
    {
        //send error back to pi to say no command was given
        Serial.println("exception");
    }
        // TODO: Create handshaking with the pi so you know that the echo has been received once that happens you can return true
    return true;
}

void setupPins()
{
    //Setup pins like this: recreate this line with the reset of the pins keep the string as the key similar to the example below
    activePins.insert(pair<string, int>("MevPin1", 1));
   
}

void runCommands()
{
    int numCommand = dataToNumber();
    switch (numCommand)
    {
    case 0:
        //logic from valve class for each valve
        flashOnce(1);//flash's are used for testing only. Get rid of for deployment
        delay(1000);
        flashOnce(1);
        commandsCompleted.push(data);
        break;
    case 1:
        flashOnce(1);
        delay(1000);
        flashTwice(1);
        commandsCompleted.push(data);
        break;
    case 2:
        flashOnce(2);
        delay(1000);
        flashOnce(1);
        delay(3000);
        commandsCompleted.push(data);
        break;
    case 3:
        flashOnce(2);
        delay(1000);
        flashTwice(1);
        delay(3000);//finish flashs later
        commandsCompleted.push(data);
        break;
    case 4:

        commandsCompleted.push(data);
        break;
    case 5: 
        commandsCompleted.push(data);
        break;
    case 6:
        commandsCompleted.push(data);
        break;
    case 7:
        commandsCompleted.push(data);
        break;
    case 8:
        commandsCompleted.push(data);
        break;
    case 9: 
        commandsCompleted.push(data);
        break;
    case 10:
        commandsCompleted.push(data);
        break;
    case 11:
        commandsCompleted.push(data);
        break;
    case 12:
        commandsCompleted.push(data);
        break;
    case 13:
        commandsCompleted.push(data);
        break;
    default:
        break;
    }
}