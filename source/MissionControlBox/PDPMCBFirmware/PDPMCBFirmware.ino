/* UVR Propulsion Development Platform Mission Control Box Firmware
 * Joshua Piuti 2022
 * 
 * This firmware controls the mission control box, gathering input from the switches and buttons, sending them to the controls computer
 * and then recieves valve state data from the computer to display on the box.
 */

//switch rows
#define R1 2
#define R2 3
#define R3 4

//columns
#define C1 7
#define C2 8
#define C3 9
#define C4 10
#define C5 11

int LEDState[10];

void setup() {
  Serial.begin(9600);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(C1, INPUT);
  pinMode(C2, INPUT);
  pinMode(C3, INPUT);
  pinMode(C4, INPUT);
  pinMode(C5, INPUT);
}//setup

void loop() {
    //TODO
    //Write new LED logic
    
    String switchStr = "SS,";
    digitalWrite(R1, HIGH);
    switchStr += valveSwitchRead("V1", C1);
    switchStr += valveSwitchRead("V2", C2);
    switchStr += valveSwitchRead("V3", C3);
    switchStr += valveSwitchRead("V4", C4);
    switchStr += valveSwitchRead("V5", C5);
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
    switchStr += valveSwitchRead("V6", C1);
    switchStr += valveSwitchRead("V7", C2);
    switchStr += valveSwitchRead("V8", C3);
    switchStr += valveSwitchRead("V9", C4);
    switchStr += valveSwitchRead("V10", C5);
    digitalWrite(R2, LOW);
    
    digitalWrite(R3, HIGH);
    if(!digitalRead(C4)){
      while(!digitalRead(C4)){
        Serial.println("ABORT");
        delay(100);
      }//while
    }//if
  
    if(digitalRead(C2)){
      Serial.println("IGNITER FIRE");
    }//if
  
    switchStr += valveSwitchRead("ME", C3);
    switchStr += valveSwitchRead("IGP", C1);
    
    digitalWrite(R3, LOW);
    Serial.println(switchStr);
}//loop

//Reads valve switches state and adds it to the string for serial comm with the MCC
String valveSwitchRead(String valveName, int valveCol){
  String valveStatus = valveName + ",";
  if(digitalRead(valveCol)){
    valveStatus += "O,";
    LEDState[valveName.substring(1).toInt() - 1] = HIGH;
  } else{
    valveStatus += "C,";
    LEDState[valveName.substring(1).toInt() - 1] = LOW;
  }//ifelse
  
  return valveStatus;
}//valveSwitchRead


//Takes Serial comm from MCC and transfers into array to output to the LEDs
void LEDParse(String led){
  //TODO write
}//LEDParse
