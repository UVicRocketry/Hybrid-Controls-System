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

//shift register
#define latchPin 12
#define clockPin 5
#define dataPin 6

byte ledTop = 0;    // Variable to hold the pattern of which LEDs are currently turned on or off
byte ledBot = 0;

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
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}//setup

void loop() {
    //TODO
    //Write new LED logic

    ledTop = 0;
    ledBot = 0;
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
    
    updateShiftRegister();
    
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
    if(valveName.substring(1).toInt() > 8){
      bitSet(ledBot, valveName.substring(1).toInt() - 9);
    } else {
      bitSet(ledTop, valveName.substring(1).toInt() - 1 );
    }//ifelse
  } else{
    valveStatus += "C,";
  }//ifelse
  
  return valveStatus;
}//valveSwitchRead

/*
 * updateShiftRegister() - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.
 */
void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, ledTop);
  shiftOut(dataPin, clockPin, LSBFIRST, ledBot);
  digitalWrite(latchPin, HIGH);
}


//Takes Serial comm from MCC and transfers into array to output to the LEDs
void LEDParse(String led){
  //TODO write
}//LEDParse
