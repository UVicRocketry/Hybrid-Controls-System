/*  Valve Cart and Feed System: Controls - Valve Controls - Full Asembly V 0.1
   Author(s):
   Logan Sewell
   JJ
   Organizarion: Uvic Rocketry
*/
#include "AccelStepper.h"
#include "Valve.h"

//*********Limit Pins*********
#define Lim1Mot1 42
#define Lim2Mot1 43
#define Lim1Mot2 41
#define Lim2Mot2 40
#define Lim1Mot3 39
#define Lim2Mot3 38
#define Lim1Mot4 36
#define Lim2Mot4 37
#define Lim1Mot5 35
#define Lim2Mot5 34
#define Lim1Mot6 33
#define Lim2Mot6 32
#define Lim1NCV 30
#define Lim2NCV 31
//****************************

//******Enable Pins **********
#define mot1Enab 46
#define mot2Enab 47
#define mot3Enab 48
#define mot4Enab 49
#define mot5Enab 50
#define mot6Enab 51

//*********Driver Pins********
#define StepMot1 3
#define DirMot1  2
#define StepMot2 5
#define DirMot2  4
#define StepMot3 7
#define DirMot3  6
#define StepMot4 9
#define DirMot4  8
#define StepMot5 11
#define DirMot5  10
#define StepMot6 27
#define DirMot6  26
//****************************

//solonoid and ignitor pin aclocation
#define Mot7 44
#define Igniter 45
//****************************

//******Assining mottors******
AccelStepper N2OF (1, StepMot1, DirMot1);
AccelStepper N2OV (1, StepMot2, DirMot2);
AccelStepper N2F (1, StepMot3, DirMot3);
AccelStepper RTV (1, StepMot4, DirMot4);
AccelStepper EVV (1, StepMot5, DirMot5);
AccelStepper MEV (1, StepMot6, DirMot6);
//*************************

//Assining Valves (be sure to set proper oriention baied on mottor direction)
Valve valve_N2OF = Valve(Lim1Mot1, Lim2Mot1);
Valve valve_N2OV = Valve(Lim2Mot2, Lim1Mot2);
Valve valve_N2F  = Valve(Lim1Mot3, Lim2Mot3);
Valve valve_RTV  = Valve(Lim1Mot4, Lim2Mot4);
Valve valve_EVV  = Valve(Lim1Mot5, Lim2Mot5);
Valve valve_MEV  = Valve(Lim1Mot6, Lim2Mot6);
Valve valve_NCV  = Valve(Lim1NCV, Lim2NCV);

int IgniterState = 0;

int TargetState [8];
uint16_t EventGroup_ValveState = 0x00;

String Buffer;
//*********************

void MoveToTarget();
String parseCommand(String * Buffer);
void setTarget (String Buffer);
void sendState (void);

void setup() {
  // Sets Limmit Pins
  pinMode(Lim1Mot1, INPUT_PULLUP);
  pinMode(Lim2Mot1, INPUT_PULLUP);
  pinMode(Lim1Mot2, INPUT_PULLUP);
  pinMode(Lim2Mot2, INPUT_PULLUP);
  pinMode(Lim1Mot3, INPUT_PULLUP);
  pinMode(Lim2Mot3, INPUT_PULLUP);
  pinMode(Lim1Mot4, INPUT_PULLUP);
  pinMode(Lim2Mot4, INPUT_PULLUP);
  pinMode(Lim1Mot5, INPUT_PULLUP);
  pinMode(Lim2Mot5, INPUT_PULLUP);
  pinMode(Lim1Mot6, INPUT_PULLUP);
  pinMode(Lim2Mot6, INPUT_PULLUP);
  pinMode(Lim1NCV, INPUT_PULLUP);
  pinMode(Lim2NCV, INPUT_PULLUP);
  //*********************************************

  //enable set up
  pinMode(mot1Enab, OUTPUT);
  pinMode(mot2Enab, OUTPUT);
  pinMode(mot3Enab, OUTPUT);
  pinMode(mot4Enab, OUTPUT);
  pinMode(mot5Enab, OUTPUT);
  pinMode(mot6Enab, OUTPUT);

  //initalize all drivers to off:
  digitalWrite(mot1Enab, LOW);
  digitalWrite(mot2Enab, LOW);
  digitalWrite(mot3Enab, LOW);
  digitalWrite(mot4Enab, LOW);
  digitalWrite(mot5Enab, LOW);
  digitalWrite(mot6Enab, LOW);

  // Set Driver Pins
  N2OF.setMaxSpeed(450.0);
  N2OF.setAcceleration(5000.0);
  N2OF.setEnablePin(mot1Enab);

  N2OV.setMaxSpeed(1000.0);
  N2OV.setAcceleration(2000.0);
  N2OV.setEnablePin(mot2Enab);

  N2F.setMaxSpeed(450.0);
  N2F.setAcceleration(5000.0);
  N2F.setEnablePin(mot3Enab);

  RTV.setMaxSpeed(400.0);
  RTV.setAcceleration(5000.0);
  RTV.setEnablePin(mot4Enab);

  EVV.setMaxSpeed(240.0);
  EVV.setAcceleration(5000.0);
  EVV.setEnablePin(mot5Enab);

  MEV.setMaxSpeed(280.0);
  MEV.setAcceleration(5000.0);
  MEV.setEnablePin(mot6Enab);

  digitalWrite(mot1Enab, HIGH);
  digitalWrite(mot2Enab, HIGH);
  digitalWrite(mot3Enab, HIGH);
  digitalWrite(mot4Enab, HIGH);
  digitalWrite(mot5Enab, HIGH);
  digitalWrite(mot6Enab, HIGH);
  N2OF.enableOutputs();
  N2OV.enableOutputs();
  N2F.enableOutputs();
  RTV.enableOutputs();
  EVV.enableOutputs();
  MEV.enableOutputs();


  pinMode(Mot7, OUTPUT);
  pinMode(Igniter, OUTPUT);
  digitalWrite(Igniter, LOW);
  //********************

  //Serial_Startup
  Serial.begin(9600);
  Serial.setTimeout(10);
  //*********************************************

  //Auto_Home(On Start up)
 // Serial.println("VCA,CF,Initalizing_to_safe_state:(Not_Implimented)");
 // digitalWrite(Mot7, LOW);


  //sendState();

  //*********************************************
}


void loop() {
  if (Serial.available())
  {
    Buffer = Serial.readString();
    String Command = "";
    String TYPE = "";
    String SOURCE = "";

   
    SOURCE = parseCommand();
    TYPE = parseCommand();

    if (TYPE == "ABORT")
    {
      Serial.println("VCA,CF,ABORT");
      digitalWrite(Igniter, LOW);
      digitalWrite(Mot7, LOW);
    }

    if (TYPE == "CTRL")
    {
      setTarget();
    }

    if(TYPE == "STATUS")
    {
      sendState();
    }
  }

  MoveToTarget();
}



void sendState (void)
{
  Serial.print("VCA,STATUS" );
  Serial.print(",N2OF," + valve_N2OF.strState());
  Serial.print(",N2OV," + valve_N2OV.strState());
  Serial.print(",N2F," + valve_N2F.strState());
  Serial.print(",RTV," + valve_RTV.strState());
  Serial.print(",EVV," + valve_EVV.strState());
  Serial.print(",MEV," + valve_MEV.strState());
  Serial.print(",NCV," + valve_NCV.strState());
  Serial.println("");
}


void MoveToTarget()
{

  if (valve_N2OF.state() != TargetState[0])
  {
    N2OF.move(TargetState[0]);
    N2OF.run();
  }
  if (valve_N2OF.getChange())
  {
    sendState();
  }

  if (valve_N2OV.state() != TargetState[1])
  {
    N2OV.move(TargetState[1]);
    N2OV.run();
  }
  if (valve_N2OV.getChange())
  {
    sendState();
  }

  if (valve_N2F.state() != TargetState[2])
  {
    N2F.move(TargetState[2]);
    N2F.run();
  }
  if (valve_N2F.getChange())
  {
    sendState();
  }

  if (valve_RTV.state() != TargetState[3])
  {
    RTV.move(TargetState[3]);
    RTV.run();
  }
  if (valve_RTV.getChange())
  {
    sendState();
  }

  if (valve_EVV.state() != TargetState[4])
  {
    EVV.move(TargetState[4]);
    EVV.run();
  }
  if (valve_EVV.getChange())
  {
    sendState();
  }

  if (valve_MEV.state() != TargetState[5])
  {
    MEV.move(TargetState[5]);
    MEV.run();
  }
  if (valve_MEV.getChange())
  {
    sendState();
  }


  if (IgniterState != TargetState[6])
  {
    if (TargetState[6] == 1)
    {
      digitalWrite(Igniter, HIGH);
      IgniterState = 1;
    } else if (TargetState[6] == 0) {
      digitalWrite(Igniter, HIGH);
      IgniterState = 0;
    }
    sendState();
  }


}

String parseCommand()
{
  String Command = "";
  char c;
  c = Buffer[0];
  while (c != '\n' && c !=  ',' && Buffer.length()>0) {
    Command += c;
    Buffer.remove(0,1);  
    c = Buffer[0];  
  }
  Buffer.remove(0,1); 
  return Command;
}


void setTarget ()
{
  String Label = "";
  String Value = "";
  int index = 0;
  while (Buffer.length() > 0)
  {
    Label = parseCommand();
    Value = parseCommand();

    if (Value == "OPEN")
    {
      TargetState[index] = 1;
    }
    else if (Value == "OPEN")
    {
      TargetState[index] = -1;
    } else
    {
      TargetState[index] = 0;
    }

    index ++;
  }
}
