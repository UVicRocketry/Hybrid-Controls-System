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
#define OpenLimitMEV 31
#define CloseLimitMEV 30
#define OpenLimitN2OV 40
#define CloseLimitN2OV 41
#define OpenLimitRTV 39
#define CloseLimitRTV 38
#define OpenLimitN2F 42
#define CloseLimitN2F 43
#define OpenLimitEVV 35
#define CloseLimitEVV 34
#define OpenLimitN2OF 33
#define CloseLimitN2OF 32
#define OpenLimitNCV 29
#define CloseLimitNCV 28
//****************************

//******Enable Pins **********
#define N2OFEnab 46
#define N2OVEnab 47
#define RTVEnab 48
#define N2FEnab 49
#define EVVEnab 50
#define MEVEnab 51

//*********Driver Pins********
#define StepMEV 19
#define DirMEV  18
#define StepN2OV 8
#define DirN2OV  7
#define StepN2F 10
#define DirN2F  9
#define StepEVV 15
#define DirEVV  14
#define StepN2OF 17
#define DirN2OF  16
//****************************

//solonoid and ignitor pin aclocation
#define Solenoid 44
#define Igniter 45
//****************************


int IgniterState = 0;
int TargetState [8];

void MoveToTarget();
void setTarget (String valveCommands);
String readCSV(String * rxBuf);
void sendState (void);


//Assining Valves (be sure to set proper oriention baied on mottor direction)
Valve N2F = Valve(OpenLimitN2F, CloseLimitN2F, StepN2F, DirN2F, 8);
Valve N2OV = Valve(OpenLimitN2OV, CloseLimitN2OV, StepN2OV, DirN2OV, 8);
Valve N2OF  = Valve(OpenLimitN2OF, CloseLimitN2OF, StepN2OF, DirN2OF, 8);
Valve RTV  = Valve(OpenLimitRTV, CloseLimitRTV);
Valve EVV  = Valve(OpenLimitEVV, CloseLimitEVV, StepEVV, DirEVV, 8);
Valve MEV  = Valve(OpenLimitMEV, CloseLimitMEV, StepMEV, DirMEV, 20);
Valve NCV  = Valve(OpenLimitNCV, CloseLimitNCV);

void setup() {
  // Sets Limmit Pins
  //enable set up
  pinMode(N2OFEnab, OUTPUT);
  pinMode(N2OVEnab, OUTPUT);
  pinMode(RTVEnab, OUTPUT);
  pinMode(N2FEnab, OUTPUT);
  pinMode(EVVEnab, OUTPUT);
  pinMode(MEVEnab, OUTPUT);


  digitalWrite(N2OFEnab, HIGH);
  digitalWrite(N2OVEnab, HIGH);
  digitalWrite(RTVEnab, LOW);
  digitalWrite(N2FEnab, HIGH);
  digitalWrite(EVVEnab, HIGH);
  digitalWrite(MEVEnab, HIGH);

  pinMode(Solenoid, OUTPUT);
  pinMode(Igniter, OUTPUT);
  digitalWrite(Igniter, LOW);
  //********************

  //Serial_Startup
  Serial.begin(9600);
  Serial.setTimeout(10);
  //*********************************************

  //Auto_Home(On Start up)
  // Serial.println("VCA,CF,Initalizing_to_safe_state:(Not_Implimented)");
  // digitalWrite(Solenoid, LOW);

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

    SOURCE = readCSV(&rxBuffer);
    TYPE = readCSV(&rxBuffer);
//    Serial.println(SOURCE);
//    Serial.println(TYPE);

    Serial.print("VCA, ACK,");
    if (TYPE == "ABORT")
    {
      Serial.println(TYPE);
      Serial.println("VCA,CF,ABORT");
      digitalWrite(Igniter, LOW);
      digitalWrite(Solenoid, LOW);
    } else if (TYPE == "CTRL")
    {
      Serial.println(TYPE);
      
      setTarget(rxBuffer);
      
    } else if (TYPE == "STATUS")
    {
      Serial.println(TYPE);
      
      sendState();
      
    } else
    {
      Serial.println("UNKOWN");
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


String readCSV (String * rxBuf)
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
    Label = readCSV(&valveCommands);
    Value = readCSV(&valveCommands);


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
      Serial.println("VCA,BADVAL");
    }

    if (Value == "OPEN")
    {
      TargetState[index] = -1;
    }
    else if (Value == "CLOSE")
    {
      TargetState[index] = 1;
    } else
    {
      TargetState[index] = 0;
      //Serial.println("VCA, NACK");
    }

  }
}





void MoveToTarget()
{

 // delay(100);
  if ( N2OF.state() != TargetState[0] && TargetState[0]!=0)
  {
    //Serial.println(TargetState[0]);
    N2OF.moveStep(TargetState[0]);
  }
  if ( N2OF.getChange())
  {
    sendState();
  }

  if ( N2OV.state() != TargetState[1] && TargetState[1]!=0)
  {
    //N2OV.moveStep(TargetState[1]);
  }
  if ( N2OV.getChange())
  {
    sendState();
  }

  if ( N2F.state() != TargetState[2] && TargetState[2]!=0)
  {
    N2F.moveStep(TargetState[2]);
  }
  if ( N2F.getChange())
  {
    sendState();
  }

  if ( RTV.state() != TargetState[3] && TargetState[3]!=0)
  {
    //RTV.moveStep(TargetState[3]);
  }
  if ( RTV.getChange())
  {
    sendState();
  }

  if ( EVV.state() != TargetState[4] && TargetState[4]!=0)
  {
    EVV.moveStep(TargetState[4]);
  }
  if ( EVV.getChange())
  {
    sendState();
  }

  if ( MEV.state() != TargetState[5] && TargetState[5]!=0)
  {
    // MEV.moveStep(TargetState[5]);
  }
  if ( MEV.getChange())
  {
    sendState();
  }

  if (IgniterState != TargetState[6] && TargetState[6]!=0)
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
