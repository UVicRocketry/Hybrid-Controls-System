/* UVR Propulsion Development Platform Mission Control Box Firmware
 * Joshua Piuti 2022
 * 
 * This firmware controls the mission control box, gathering input from the switches and buttons, sending them to the controls computer
 * and then recieves valve state data from the computer to display on the box.
 */

//matrix row pins
#define R1 2
#define R2 3
#define R3 4

//matrix columns pins
#define C1 7
#define C2 8
#define C3 9
#define C4 10
#define C5 A0

//shift register pins
#define latchPin 12
#define clockPin 5
#define dataPin 6

//valve aliases
String V1 = "N2OF";
String V2 = "N2OV";
String V3 = "N2F";
String V4 = "RTV";
String V5 = "NCV";
String V6 = "EVV";
String V7 = "V7";
String V8 = "V8";
String V9 = "V9";
String V10 = "V10";

String * valveList[10];

byte ledTop = 0;    // Holds which LEDs are currently turned on or off for first shift register
byte ledBot = 0;    // Holds LEDs for second shift register

void setup() {
  Serial.begin(9600);
  while (!Serial){};

  //sets pinmodes
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

  //adds valve names to list for string processing
  valveList[0] = & V1;
  valveList[1] = & V2;
  valveList[2] = & V3;
  valveList[3] = & V4;
  valveList[4] = & V5;
  valveList[5] = & V6;
  valveList[6] = & V7;
  valveList[7] = & V8;
  valveList[8] = & V9;
  valveList[9] = & V10;
  
}//setup

void loop() {
    //resets shift register bytes
    ledTop = 0;
    ledBot = 0;

    //goes throught switches and polls them for their state then adds
    //them to the output string
    String switchStr = "CBX,SS,";
    digitalWrite(R1, HIGH);
    switchStr += valveSwitchRead(V1, C1);
    switchStr += valveSwitchRead(V2, C2);
    switchStr += valveSwitchRead(V3, C3);
    switchStr += valveSwitchRead(V4, C4);
    switchStr += valveSwitchRead(V5, C5);
    digitalWrite(R1, LOW);
    digitalWrite(R2, HIGH);
    switchStr += valveSwitchRead(V6, C1);
    switchStr += valveSwitchRead(V7, C2);
    switchStr += valveSwitchRead(V8, C3);
    switchStr += valveSwitchRead(V9, C4);
    switchStr += valveSwitchRead(V10, C5);
    digitalWrite(R2, LOW);
    
    updateShiftRegister();

    //Checks if abort button is pressed, if so sends ABORT signal until it is depressed
    digitalWrite(R3, HIGH);
    if(!digitalRead(C4)){
      while(!digitalRead(C4)){
        Serial.println("ABORT");
        delay(100);
      }//while
    }//if

    //Only allows commands other than ABORT to be sent to the computer if arming key is inserted and engaged
    if(!digitalRead(C5)){
      
    }

    //Checks if the igniter fire button is depressed and sends IGNITER FIRE signal
    if(digitalRead(C2)){
      Serial.println("IGNITE");
    }//if

    //Adds MEV and IGP switch to output string
    switchStr += valveSwitchRead("MEV", C3);
    switchStr += valveSwitchRead("IGP", C1);
    digitalWrite(R3, LOW);

    //Sends output string over serial
    Serial.println(switchStr);
    
    //Reads input string from serial
    String valveState = Serial.readString();
    
    //Checks if header is present
    int index = valveState.indexOf("MCC,CD");
    if(index != -1){
      int i;
  
      //Goes through input string and turns on LEDs according to which valves are  open
      for(i = 0; i < 10; i++){
        valveParse(valveState, *valveList[i]);
      }//for
    }//if
    delay(100);
    
}//loop


//Functions
//-----------------------------------------------------------------------------------------------
//Reads valve switches state and adds it to the string for serial comm with the MCC
String valveSwitchRead(String valveName, int valveCol){
  String valveStatus = valveName + ",";
  if(digitalRead(valveCol)){
    valveStatus += "OPEN,";
    
    //This if statement is only for testing if the switches are working,
    //simulates input from computer to turn on leds
    ///*
    if(valveName.substring(1).toInt() > 8){
      bitSet(ledBot, valveName.substring(1).toInt() - 9);
    } else {
      bitSet(ledTop, valveName.substring(1).toInt() - 1 );
    }//ifelse
    //*/
  } else{
    valveStatus += "CLOSE,";
  }//ifelse
  
  return valveStatus;
}//valveSwitchRead

//Parses 
void valveParse (String input, String valveName){
   //Finds index of valveName in input string
   int index = input.indexOf(valveName + ",");

   //If index is found check for valve position
   if(index != -1){
    int end = input.indexOf(",", valveName.length() + 1);
    String value = input.substring(index + valveName.length() + 1, end);

    //If valve is open, set bit in LED byte for that valve to ON
    if(value == "OPEN"){
      if(valveName.substring(1).toInt() > 8){
        bitSet(ledBot, valveName.substring(1).toInt() - 9);
      } else {
        bitSet(ledTop, valveName.substring(1).toInt() - 1 );
      }//ifelse
    }//if
   }//if
}//valveParse


//updateShiftRegister() - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.
//Found it in a tutorial for shift registers - https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/
void updateShiftRegister() {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, ledTop);
  shiftOut(dataPin, clockPin, LSBFIRST, ledBot);
  digitalWrite(latchPin, HIGH);
}//updateShiftRegisters
