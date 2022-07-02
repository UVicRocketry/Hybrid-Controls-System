/* UVR Propulsion Development Platform Mission Control Box Firmware
  Joshua Piuti 2022

  This firmware controls the mission control box, gathering input from the switches and buttons, sending them to the controls computer
  and then recieves valve state data from the computer to display on the box.
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

//buzzer pin
#define buzzer A1

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

bool armed = false; //to check whether system is armed for key turn buzzer
bool fired = false; //to check if MEV is open, then turn off primed buzzer

bool testing = false; //set to true to put box into testing mode, makes LEDs turn on due to switches


void setup() {
  Serial.begin(9600);
  while (!Serial) {};

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
  pinMode(buzzer, OUTPUT);

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
  //goes throught switches and polls them for their state then adds
  //them to the output string
  String switchStr = "CBX,CD,";
  String NCV = "";
  digitalWrite(R1, HIGH);
  switchStr += valveSwitchRead(V1, C1, 0);
  switchStr += valveSwitchRead(V2, C2, 1);
  switchStr += valveSwitchRead(V3, C3, 2);
  switchStr += valveSwitchRead(V4, C4, 3);
  NCV += valveSwitchRead(V5, C5, 4);
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  switchStr += valveSwitchRead(V6, C1, 5);
  switchStr += valveSwitchRead(V7, C2, 6);
  switchStr += valveSwitchRead(V8, C3, 7);
  switchStr += valveSwitchRead(V9, C4, 8);
  switchStr += valveSwitchRead(V10, C5, 9);
  digitalWrite(R2, LOW);

  //Checks if abort button is pressed, if so sends ABORT signal until it is depressed
  digitalWrite(R3, HIGH);
  if (!digitalRead(C4)) {
    while (!digitalRead(C4)) {
      Serial.println("CBX,CD,ABORT,,");
      delay(100);
    }//while
  }//if

  //checks if key has been turned, then turns on buzzer if it has been
  if (digitalRead(C5) && !armed) {
    tone(buzzer, 300, 500);
    armed = true;
  } else if (!digitalRead(C5)) {
    armed = false;
  }//else if

  //Only allows commands other than ABORT to be sent to the computer if arming key is inserted and engaged
  if (digitalRead(C5)) {

    updateShiftRegister();

    //Checks if the igniter fire button is depressed and sends IGNITER FIRE signal
    while (digitalRead(C2)) {
      Serial.println("CBX,CD,IGNITE,,");
      delay(500);
    }//if

    //Adds MEV and IGP switch to output string
    switchStr += valveSwitchRead("IGP", C1, -1);
    switchStr += valveSwitchRead("MEV", C3, -1);
    switchStr += NCV;

    //checks whether MEV switch has been activated, so primed buzzer can be turned off
    if (digitalRead(C3)) {
      fired = true;
    } else {
      fired = false;
    }//if else

    //if system is primed for ignition, turns on buzzer, if
    if (digitalRead(C1) && !fired) {
      tone(buzzer, 500, 1200);
    } else {
      //noTone(buzzer);
    }//if else
    digitalWrite(R3, LOW);

    //Sends output string over serial
    Serial.println(switchStr);

    ///*
    //Reads feedback data from serial
    //if(Serial.available()){
    String valveState = Serial.readString();//Until('\n');

    //Checks if header is present
    int index = valveState.indexOf("MCC,FD");
    if (index != -1) {
      int i;
      ledTop = 0;
      ledBot = 0;

      //Goes through feedback data and turns on LEDs according to which valves are  open
      for (i = 0; i < 10; i++) {
        valveParse(valveState, *valveList[i], i);
      }//for
    }//if
    //*/
  }//if
  //}//if

  delay(100);
}//loop


//Functions
//-----------------------------------------------------------------------------------------------
//Reads valve switches state and adds it to the string for serial comm with the MCC
//pos is just for testing box functionality without a connected computer sending feedback data
String valveSwitchRead(String valveName, int valveCol, int pos) {
  String valveStatus = valveName + ",";
  if (digitalRead(valveCol)) {
    valveStatus += "OPEN,";

    //This if statement is only for testing if the switches are working,
    //simulates input from computer to turn on leds
    if (testing) {
      if (pos < 0) {
      } else if (pos > 7) {
        bitSet(ledBot, pos - 8);
      } else {
        bitSet(ledTop, pos);
      }//ifelse
    }//if

  } else {
    valveStatus += "CLOSE,";
  }//ifelse

  return valveStatus;
}//valveSwitchRead

//Parses feedback data to turn on LEDs
void valveParse (String input, String valveName, int pos) {
  //Finds index of valveName in input string
  int index = input.indexOf(valveName + ",");
  //If index is found check for valve position
  if (index != -1) {
    int end = input.indexOf(",", index + valveName.length() + 1);
    String value = input.substring(index + valveName.length() + 1, end);
    //If valve is open, set bit in LED byte for that valve to ON
    if (value == "OPEN") {
      if (pos > 7) {
        bitSet(ledBot, pos - 8);
      } else {
        bitSet(ledTop, pos);
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
