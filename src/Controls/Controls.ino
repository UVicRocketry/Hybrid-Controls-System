/*  Valve Cart and Feed System: Controls - Valve Controls - Full Asembly V 1.1
   Author(s):
   Matthew Ebert
   Logan Sewell
   JJ
   Organizarion: UVic Rocketry
*/
//#include "AccelStepper.h"
#include "Valve.h"

//*********Limit Pins*********
#define Lim1Mot6 31
#define Lim2Mot6 30
#define Lim1Mot2 41
#define Lim2Mot2 40
#define Lim1Mot3 39
#define Lim2Mot3 38
#define Lim1Mot4 43
#define Lim2Mot4 42
#define Lim1Mot5 35
#define Lim2Mot5 34
#define Lim1Mot1 33
#define Lim2Mot1 32
#define Lim1NCV 29
#define Lim2NCV 28
//****************************

//******Enable Pins **********
#define mot1Enab 46
#define mot2Enab 47
#define mot3Enab 48
#define mot4Enab 49
#define mot5Enab 50
#define mot6Enab 51

//*********Driver Pins********
#define StepMot6 19
#define DirMot6  18
#define StepMot2 8
#define DirMot2  7
#define StepMot4 10
#define DirMot4  9
#define StepMot5 15
#define DirMot5  14
#define StepMot1 17
#define DirMot1  16
//****************************

//solonoid and ignitor pin aclocation
#define Mot7 44
#define Igniter 45
//****************************




int IgniterState = 0;
int TargetState [8];

void MoveToTarget();
void setTarget (String valveCommands);
String readValue(String * rxBuf);
void sendState (void);


//Assining Valves (be sure to set proper oriention baied on mottor direction)
Valve N2F = Valve(Lim2Mot4, Lim1Mot4, StepMot4, DirMot4, 15);
Valve N2OV = Valve(Lim2Mot2, Lim1Mot2, StepMot2, DirMot2, 15);
Valve N2OF  = Valve(Lim1Mot1, Lim2Mot1, StepMot1, DirMot1, 15);
Valve RTV  = Valve(Lim1Mot3, Lim2Mot3);
Valve EVV  = Valve(Lim1Mot5, Lim2Mot5, StepMot5, DirMot5, 20);
Valve MEV  = Valve(Lim1Mot6, Lim2Mot6, StepMot6, DirMot6, 20);
Valve NCV  = Valve(Lim1NCV, Lim2NCV);

void setup() {
  // Sets Limmit Pins
  //enable set up
  pinMode(mot1Enab, OUTPUT);
  pinMode(mot2Enab, OUTPUT);
  pinMode(mot3Enab, OUTPUT);
  pinMode(mot4Enab, OUTPUT);
  pinMode(mot5Enab, OUTPUT);
  pinMode(mot6Enab, OUTPUT);


  digitalWrite(mot1Enab, HIGH);
  digitalWrite(mot2Enab, HIGH);
  digitalWrite(mot3Enab, LOW);
  digitalWrite(mot4Enab, HIGH);
  digitalWrite(mot5Enab, HIGH);
  digitalWrite(mot6Enab, HIGH);

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

  sendState();

  //*********************************************
}


void loop() {
  if (Serial.available())
  {
    String rxBuffer = Serial.readString();
    String Command = "";
    String TYPE = "";
    String SOURCE = "";


    SOURCE = readValue(&rxBuffer);
    TYPE = readValue(&rxBuffer);
    Serial.println(SOURCE);
    Serial.println(TYPE);
    if (TYPE == "ABORT")
    {
      Serial.println("VCA, ACK");
      Serial.println("VCA,CF,ABORT");
      digitalWrite(Igniter, LOW);
      digitalWrite(Mot7, LOW);
    } else if (TYPE == "CTRL")
    {
      Serial.println("VCA, ACK");
   
      setTarget(rxBuffer);
  
    } else if (TYPE == "STATUS")
    {
      Serial.println("VCA, ACK");
      sendState();
    } else
    {
      Serial.println("VCA, NACK");
    }


  }

  MoveToTarget();
}


/***********FUNCTIONS*******************/


void sendState (void)
{
  Serial.print("VCA,STATUS" );
  Serial.print(",N2OF," +  N2OF.strState());
  Serial.print(",N2OV," +  N2OV.strState());
  Serial.print(",N2F," +  N2F.strState());
  Serial.print(",RTV," +  RTV.strState());
  Serial.print(",EVV," +  EVV.strState());
  Serial.print(",MEV," +  MEV.strState());
  Serial.print(",NCV," +  NCV.strState());
  Serial.println("");
}


String readValue (String * rxBuf)
{
  String Command = "";
  char c;
  c = (*rxBuf)[0];
  while (c != '\n' && c !=  ',' && (*rxBuf).length() > 1) {
    Command += c;
    (*rxBuf).remove(0, 1);
    c = (*rxBuf)[0];
  }
  (*rxBuf).remove(0, 1);
  return Command;
}


void setTarget (String valveCommands)
{
  String Label = "";
  String Value = "";
  int index = 0;
  while (valveCommands.length() > 1)
  {
    Label = readValue(&valveCommands);
    Value = readValue(&valveCommands);


    if (Label == "N2OF")
    {
    } else   if (Label ==  "N2OV")
    {
      index = 1;
    } else if (Label ==  "N2F")
    {
      index = 2;
    } else if (Label ==  "RTV")
    {
      index = 3;
    } else if (Label ==  "EVV")
    {
      index = 4;
    } else if (Label ==  "MEV")
    {
      index = 5;
    } else if (Label ==  "IGNITE")
    {
      index = 6;
    } else
    {
      index = 7;
      Serial.println("VCA, NACK");
    }

    if (Value == "OPEN")
    {
      TargetState[index] = 1;
    }
    else if (Value == "CLOSE")
    {
      TargetState[index] = -1;
    } else
    {
      TargetState[index] = 0;
      //Serial.println("VCA, NACK");
    }

  }
}





void MoveToTarget()
{

  if ( N2OF.state() != TargetState[0] && TargetState[0]!=0)
  {
    N2OF.moveStep(TargetState[0]);
  }
  if ( N2OF.getChange())
  {
    sendState();
  }

  if ( N2OV.state() != TargetState[1]&& TargetState[0]!=0)
  {
    N2OV.moveStep(TargetState[1]);
  }
  if ( N2OV.getChange())
  {
    sendState();
  }

  if ( N2F.state() != TargetState[2]&& TargetState[0]!=0)
  {
    N2F.moveStep(TargetState[2]);
  }
  if ( N2F.getChange())
  {
    sendState();
  }

  if ( RTV.state() != TargetState[3]&& TargetState[0]!=0)
  {
    //RTV.moveStep(TargetState[3]);
  }
  if ( RTV.getChange())
  {
    sendState();
  }

  if ( EVV.state() != TargetState[4]&& TargetState[0]!=0)
  {
    EVV.moveStep(TargetState[4]);
  }
  if ( EVV.getChange())
  {
    sendState();
  }

  if ( MEV.state() != TargetState[5]&& TargetState[0]!=0)
  {
    // MEV.moveStep(TargetState[5]);
  }
  if ( MEV.getChange())
  {
    sendState();
  }

  if (IgniterState != TargetState[6]&& TargetState[0]!=0)
  {
    if (TargetState[6] == 1)
    {
      //digitalWrite(Igniter, HIGH);
      IgniterState = 1;
    } else if (TargetState[6] == 0) {
      //digitalWrite(Igniter, HIGH);
      IgniterState = 0;
    }
    sendState();
  }
}
