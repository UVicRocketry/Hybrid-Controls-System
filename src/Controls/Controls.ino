/*  Valve Cart and Feed System: Controls - Valve Controls - Rework V 1.1
   Author(s):
   Matthew Ebert
   Logan Sewell
   JJ
   Organization: UVic Rocketry
*/
#include "Valve.h"

//*********Limit Pins*********
#define OpenLimitMEV 30
#define CloseLimitMEV 31
#define OpenLimitN2OV 40
#define CloseLimitN2OV 41
#define OpenLimitRTV 39
#define CloseLimitRTV 38
#define OpenLimitN2F 42
#define CloseLimitN2F 43
#define OpenLimitERV 35
#define CloseLimitERV 34
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
#define ERVEnab 50
#define MEVEnab 51

//*********Driver Pins********
#define StepMEV 19
#define DirMEV  18
#define StepN2OV 8
#define DirN2OV  7
#define StepN2F 10
#define DirN2F  9
#define StepERV 15
#define DirERV  14
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

bool ABORT = 0;
//Assining Valves
//Stepper Valves
Valve N2F = Valve(OpenLimitN2F, CloseLimitN2F, StepN2F, DirN2F, 20);
Valve N2OV = Valve(OpenLimitN2OV, CloseLimitN2OV, StepN2OV, DirN2OV, 8);
Valve N2OF  = Valve(OpenLimitN2OF, CloseLimitN2OF, StepN2OF, DirN2OF, 8);
Valve ERV  = Valve(OpenLimitERV, CloseLimitERV, StepERV, DirERV, 8);
Valve MEV  = Valve(OpenLimitMEV, CloseLimitMEV, StepMEV, DirMEV, 100);

//Other Valves
Valve NCV  = Valve(OpenLimitNCV, CloseLimitNCV);
Valve RTV  = Valve(OpenLimitRTV, CloseLimitRTV);

void setup() {
  // Sets Limmit Pins
  //enable set up
  pinMode(N2OFEnab, OUTPUT);
  pinMode(N2OVEnab, OUTPUT);
  pinMode(N2FEnab, OUTPUT);
  pinMode(ERVEnab, OUTPUT);
  pinMode(MEVEnab, OUTPUT);
  pinMode(RTVEnab, OUTPUT);

  //Currently does not effect motors??
  digitalWrite(N2OFEnab, HIGH);
  digitalWrite(N2OVEnab, HIGH);
  digitalWrite(N2FEnab, HIGH);
  digitalWrite(ERVEnab, HIGH);
  digitalWrite(MEVEnab, HIGH);
  digitalWrite(RTVEnab, LOW);

  //Unsure how these operate
  pinMode(Solenoid, OUTPUT);
  digitalWrite(Solenoid, LOW);
  pinMode(Igniter, OUTPUT);
  digitalWrite(Igniter, LOW);
  //********************


  //Serial_Startup
  Serial.begin(9600);
  //time in ms the serail blocks waiting for data
  Serial.setTimeout(10);
  //*********************************************

  Serial.println("VCA,STATUS,STARTUP,SUCCESS");
  Serial.println("VCA,CF,Initalizing_to_safe_state:(ALL Closed)");
  sendState();
  delay(1000);
  //Set all target states to closed
  for(int i = 0; i<8; i++)
  {
    TargetState[i] = 1;
  }
}


void loop() {
  
  if (Serial.available())
  {//if a message has been recieved
    //read all serial data
    String rxBuffer = Serial.readString();
    /*Should the buffer be cleared before entire message is sent/recieved
    a fragment will be created and not parsed. An end flag should be checked
    to ensure this case does not occur.*/
    String Command = "";
    String TYPE = "";
    String SOURCE = "";
    //read source of data
    SOURCE = readCSV(&rxBuffer);
    //read type of data
    TYPE = readCSV(&rxBuffer);
    //Send acknolodgement
    Serial.print("VCA, ACK,");
    /* Currently there is no error correcting or proper acknoledgement handling
    This ACK is for debugging only. Proper protocols should be developed to ensure
    a message is handled properly*/
    if (TYPE == "ABORT")
    {
      ABORT = true;
      Serial.println(TYPE);
      Serial.println("VCA,CF,ABORT");
      digitalWrite(Igniter, LOW);
      digitalWrite(Solenoid, LOW);

      //implement abort code
      
      Serial.println("VCA,ABORTED");
      sendState();
      
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

  //called every loop to step motors if necessary
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
  Serial.print(",ERV," +  ERV.strState());
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
    
    } else if (Label ==  "ERV")
    {
      index = 3;
    } else if (Label ==  "MEV")
    {
      index = 4;
    } else if (Label ==  "RTV")
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
    }

  }
}





void MoveToTarget()
{

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
  //  delay(1);
   // N2OV.moveStep(TargetState[1]);
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

  if ( ERV.state() != TargetState[3] && TargetState[3]!=0)
  {
    ERV.moveStep(TargetState[3]);
  }
  if ( ERV.getChange())
  {
    sendState();
  }

  if ( MEV.state() != TargetState[4] && TargetState[4]!=0)
  {
    delay(1);
    MEV.moveStep(TargetState[4]);
  }
  
  if ( MEV.getChange())
  {
    sendState();
  }


  if ( RTV.state() != TargetState[5] && TargetState[5]!=0)
  {
    RTV.moveStep(TargetState[5]);
  }
  if ( RTV.getChange())
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
