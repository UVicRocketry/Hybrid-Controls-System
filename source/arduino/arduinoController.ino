#include <QueueList.h>
/* 
If the library does import follow this https://playground.arduino.cc/Code/QueueList/
*/
String data;

QueueList<String> commandsCompleted;
bool next_value;

void setup() 
{
    pinMode(13, OUTPUT);
    Serial.begin(115200);
}
//these two functions are used to arduino LED feedback
void flash_once();
void flash_twice();

//This function sends the top value from commandsCompleted and returns true once its been sent
bool send_data();

void run_commands(); 

void loop()
{

    digitalWrite(13, LOW);

    if(Serial.available())
    {
        data = Serial.readString();

        /*
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

        Contact Ben S or Kris L for more
        */

        //igniter control flow here
        run_commands()
        next_value = !commandsCompleted.isEmpty() : true ? false;
    }

    if(!commandsCompleted.isEmpty() && next_value)
    {
        next_value = send_data()
    } 
    else
    {
        next_value = false;
    }

}



//function defintions below 

void run_commands(){

    if(data == "MEV closed")
    { //1
        digitalWrite(13, LOW);
        commandsCompleted.push(data);
        flash_once();
        delay(1000); //gap between sets of flashes

        flash_once();
        delay(3000); //gap before next first set of flashes
    }

    else if(data == "MEV open")
    { 
        digitalWrite(13, LOW);
        commandsCompleted(data);
        
        flash_once();
        delay(1000);
        
        flash_twice();
        delay(3000);
    }

    else if(data == "N2OV closed")
    { //2

        digitalWrite(13, LOW);
        commandsCompleted.push(data);

        flash_once();
        flash_once();
        delay(1000);

        flash_once();
        delay(3000);
    }

    else if(data == "N2OV open")
    { 
        digitalWrite(13, LOW);
        commandsCompleted.push(data);

        flash_once();
        flash_once();
        delay(1000);
        
        flash_twice();
        delay(3000);
    }

    else if(data == "N2O closed")
    { //3
        digitalWrite(13, LOW);
        commandsCompleted.push(data);

        for(int i = 1; i <=3; i++) flash_once();
        delay(1000);

        flash_once();
        delay(3000);
    }

    else if(data == "N2O open")
    { 
        digitalWrite(13, LOW);
        commandsCompleted.push(data);

        for(int i = 1; i <=3; i++) flash_once();
        delay(1000);
        
        flash_twice();
        delay(3000);
    }

    else if(data == "N2 closed")
    { //4
        digitalWrite(13, LOW);
        commandsCompleted.push(data);

        for(int i = 1; i <=4; i++) flash_once();
        delay(1000);

        flash_once();
        delay(3000);
    }

    else if(data == "N2 open")
    { 
        digitalWrite(13, LOW);
        commandsCompleted.push(data);
        
        for(int i = 1; i <=4; i++) flash_once();
        delay(1000);
        
        flash_twice();
        delay(3000);
    }

    else if(data == "NCV closed")
    { //5
        digitalWrite(13, LOW);
        commandsCompleted.push(data);

        for(int i = 1; i <=5; i++) flash_once();
        delay(1000);

        flash_once();
        delay(3000);
    }

    else if(data == "NCV open")
    {
        digitalWrite(13, LOW);
        commandsCompleted.push(data);
        
        for(int i = 1; i <=5; i++) flash_once();
        delay(1000);
        
        flash_twice();
        delay(3000);
    }

    else if(data == "RV closed")
    { //6
        digitalWrite(13, LOW);
        commandsCompleted.push(data);

        for(int i = 1; i <=6; i++) flash_once();
        delay(1000);

        flash_once();
        delay(3000);
    }

    else if(data == "RV open")
    {
        digitalWrite(13, LOW);
        commandsCompleted.push(data);
        
        for(int i = 1; i <=6; i++) flash_once();
        delay(1000);
        
        flash_twice();
        delay(3000);
    }

    else if(data == "N2V closed")
    { //7
        digitalWrite(13, LOW);
        commandsCompleted.push(data);

        for(int i = 1; i <=7; i++) flash_once();
        delay(1000);

        flash_once();
        delay(3000);
    }

    else if(data == "N2V open")
    {
        digitalWrite(13, LOW);
        commandsCompleted.push(data);
        
        for(int i = 1; i <=7; i++) flash_once();
        delay(1000);
        
        flash_twice();
        delay(3000);
    }

}

bool send_data() 
{
    Serial.println(commandsCompleted.pop());
    // TODO: Create handshaking with the pi so you know that the echo has been received once that happens you can return true
    return true;
}

void flash_twice() 
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

void flash_once() 
{ 
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
}