///*  Valve Cart and Feed System: Controls - Valve Controls - Full Asembly V 0.1
//   Author(s):
//   Logan Sewell
//   JJ
//   Organizarion: Uvic Rocketry
//*/
//#include "AccelStepper.h"
//#include "Valve.h"
//
////*********Limit Pins*********
//#define Lim1Mot1 42
//#define Lim2Mot1 43
//#define Lim1Mot2 41
//#define Lim2Mot2 40
//#define Lim1Mot3 39
//#define Lim2Mot3 38
//#define Lim1Mot4 36
//#define Lim2Mot4 37
//#define Lim1Mot5 35
//#define Lim2Mot5 34
//#define Lim1Mot6 33
//#define Lim2Mot6 32
//#define Lim1NCV 30
//#define Lim2NCV 31
////****************************
//
////******Enable Pins **********
//#define mot1Enab 46
//#define mot2Enab 47
//#define mot3Enab 48
//#define mot4Enab 49
//#define mot5Enab 50
//#define mot6Enab 51
//
////*********Driver Pins********
//#define StepMot1 3
//#define DirMot1  2
//#define StepMot2 5
//#define DirMot2  4
//#define StepMot3 7
//#define DirMot3  6
//#define StepMot4 9
//#define DirMot4  8
//#define StepMot5 11
//#define DirMot5  10
//#define StepMot6 27
//#define DirMot6  26
////****************************
//
////solonoid and ignitor pin aclocation
//#define Mot7 44
//#define Igniter 45
////****************************
//
//
////******Assining mottors******
//AccelStepper N2OF (1, StepMot1, DirMot1);
//AccelStepper N2OV (1, StepMot2, DirMot2);
//AccelStepper N2F (1, StepMot3, DirMot3);
//AccelStepper RTV (1, StepMot4, DirMot4);
//AccelStepper EVV (1, StepMot5, DirMot5);
//AccelStepper MEV (1, StepMot6, DirMot6);
////*************************
//
////Assining Valves (be sure to set proper oriention baied on mottor direction)
//Valve valve_N2OF = Valve(Lim1Mot1, Lim2Mot1);
//Valve valve_N2OV = Valve(Lim2Mot2, Lim1Mot2);
//Valve valve_N2F  = Valve(Lim1Mot3, Lim2Mot3);
//Valve valve_RTV  = Valve(Lim1Mot4, Lim2Mot4);
//Valve valve_EVV  = Valve(Lim1Mot5, Lim2Mot5);
//Valve valve_MEV  = Valve(Lim1Mot6, Lim2Mot6);
//Valve valve_NCV  = Valve(Lim1NCV, Lim2NCV);
//
////*********************
//
//void setState(int s1, int s2, int s3, int s4, int s5, int s6) {
//  digitalWrite(mot1Enab, HIGH);
//  digitalWrite(mot2Enab, HIGH);
//  digitalWrite(mot3Enab, HIGH);
//  digitalWrite(mot4Enab, HIGH);
//  digitalWrite(mot5Enab, HIGH);
//  digitalWrite(mot6Enab, HIGH);
//
//
//  N2OF.enableOutputs();
//  N2OV.enableOutputs();
//  N2F.enableOutputs();
//  RTV.enableOutputs();
//  EVV.enableOutputs();
//  MEV.enableOutputs();
//
//
//  N2OF.moveTo(N2OF.currentPosition() + s1 * (50000000));
//  N2OV.moveTo(N2OV.currentPosition() + s2 * 50000);
//  N2F.moveTo(N2F.currentPosition()  + s3 * 50000);
//  RTV.moveTo(RTV.currentPosition()  + s4 * 50000);
//  EVV.moveTo(EVV.currentPosition()  + s5 * 50000);
//  MEV.moveTo(MEV.currentPosition()  + s6 * 50000);
//
//  unsigned long temp = micros();
//  //x is a time veriable used to regulate the step time of the mottors as they pulse
//  int x;
//
//  //n is the repetition veriable that detects if any error has ocurred
//  long n = 0;
//
//  while (n < 50000 && (valve_N2OF.state() != s1 || valve_N2OV.state() != s2 || valve_N2F.state() != s3 || valve_RTV.state() != s4 || valve_EVV.state() != s5 || valve_MEV.state() != s6)) {
//    n++;
//    temp = micros();
//
//    if (valve_N2OF.state() != s1) {
//      N2OF.run();
//    }
//
//    if (valve_N2OV.state() != s2) {
//      N2OV.run();
//    }
//
//    if (valve_N2F.state() != s3) {
//      N2F.run();
//    }
//
//    if (valve_RTV.state() != s4) {
//      RTV.run();
//    }
//
//    if (valve_EVV.state() != s5) {
//      EVV.run();
//    }
//
//    if (valve_MEV.state() != s6) {
//      MEV.run();
//    }
//
//  }
//
//  if (valve_N2OF.state() != s1 || valve_N2OV.state() != s2 || valve_N2F.state() != s3 || valve_RTV.state() != s4 || valve_EVV.state() != s5 || valve_MEV.state() != s6) {
//
//    Serial.print("VCA,ER" );
//    Serial.print(",N2OF," + valve_N2OF.strState());
//    Serial.print(",N2OV," + valve_N2OV.strState());
//    Serial.print(",N2F," + valve_N2F.strState());
//    Serial.print(",RTV," + valve_RTV.strState());
//    Serial.print(",EVV," + valve_EVV.strState());
//    Serial.print(",MEV," + valve_MEV.strState());
//    Serial.print(",NCV," + valve_NCV.strState());
//    Serial.println("");
//  }
//  N2OF.disableOutputs();
//  N2OV.disableOutputs();
//  N2F.disableOutputs();
//  RTV.disableOutputs();
//  EVV.disableOutputs();
//  MEV.disableOutputs();
//
//}
//
////***********************Reads-Searial-Values-one-comand-at-a-time***********
////reades a String in serial comunication format and reads the next command
//String readSerial(int cur, String Buffer) {
//  if (cur > Buffer.length()) {
//    return "Error:_End_of_String";
//  }
//  String Command;
//  char c;
//  while (c != '\n' && c !=  ',') {
//    c = Buffer[cur];
//    if (c != '\n' && c !=  ',') {
//      Command += c;
//    }
//    cur++;
//  }
//  return Command;
//}
//
////*********************Set-Valves-to-serial-state***************************
//void setSerial(int cur, String Buffer) {
//
//  String Command;
//
//  //************** Valve 1 *********************
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//  int V1 = 0;
//  if (Command.compareTo("OPEN") == 0) {
//    V1 = -1;
//  } else if (Command.compareTo("CLOSE") == 0) {
//    V1 = 1;
//  } else {
//    //Serial.println("Error Incorect imput 1: [" + Command + "]");
//  }
//
//  //************** Valve 2 *********************
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//  int V2 = 0;
//  if (Command.compareTo("OPEN") == 0) {
//    V2 = -1;
//  } else if (Command.compareTo("CLOSE") == 0) {
//    V2 = 1;
//  } else {
//    //Serial.println("Error Incorect imput 2: [" + Command + "]");
//  }
//
//  //************** Valve 3 *********************
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//  int V3 = 0;
//  if (Command.compareTo("OPEN") == 0) {
//    V3 = -1;
//  } else if (Command.compareTo("CLOSE") == 0) {
//    V3 = 1;
//  } else {
//    //Serial.println("Error Incorect imput 3: [" + Command + "]");
//  }
//
//  //************** Valve 4 *********************
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//  int V4 = 0;
//  if (Command.compareTo("OPEN") == 0) {
//    V4 = -1;
//  } else if (Command.compareTo("CLOSE") == 0) {
//    V4 = 1;
//  } else {
//    //Serial.println("Error Incorect imput 4: [" + Command + "]");
//  }
//
//  //************** Valve 5 *********************
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//  int V5 = 0;
//  if (Command.compareTo("OPEN") == 0) {
//    V5 = -1;
//  } else if (Command.compareTo("CLOSE") == 0) {
//    V5 = 1;
//  } else {
//    //Serial.println("Error Incorect imput 5: [" + Command + "]");
//  }
//
//  //************** Valve 6 *********************
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//  int V6 = 0;
//  if (Command.compareTo("OPEN") == 0) {
//    V6 = -1;
//  } else if (Command.compareTo("CLOSE") == 0) {
//    V6 = 1;
//  } else {
//    //Serial.println("Error Incorect imput 6: [" + Command + "]");
//  }
//
//  //************** Valve 7 *********************
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//  int V7 = 0;
//  if (Command.compareTo("OPEN") == 0) {
//    V7 = -1;
//    digitalWrite(Mot7, HIGH);
//  } else if (Command.compareTo("CLOSE") == 0) {
//    V7 = 1;
//    digitalWrite(Mot7, LOW);
//  } else {
//    //Serial.println("Error Incorect imput 6: [" + Command + "]");
//  }
//
//  setState(V1, V2, V3, V4, V5, V6);
//
//  Serial.print("VCA,FD" );
//  Serial.print(",N2OF," + valve_N2OF.strState());
//  Serial.print(",N2OV," + valve_N2OV.strState());
//  Serial.print(",N2F," + valve_N2F.strState());
//  Serial.print(",RTV," + valve_RTV.strState());
//  Serial.print(",EVV," + valve_EVV.strState());
//  Serial.print(",MEV," + valve_MEV.strState());
//  Serial.print(",NCV," + valve_NCV.strState());
//  Serial.println("");
//}
//
//
//void setup() {
//  // Sets Limmit Pins
//  pinMode(Lim1Mot1, INPUT_PULLUP);
//  pinMode(Lim2Mot1, INPUT_PULLUP);
//
//  pinMode(Lim1Mot2, INPUT_PULLUP);
//  pinMode(Lim2Mot2, INPUT_PULLUP);
//
//  pinMode(Lim1Mot3, INPUT_PULLUP);
//  pinMode(Lim2Mot3, INPUT_PULLUP);
//
//  pinMode(Lim1Mot4, INPUT_PULLUP);
//  pinMode(Lim2Mot4, INPUT_PULLUP);
//
//  pinMode(Lim1Mot5, INPUT_PULLUP);
//  pinMode(Lim2Mot5, INPUT_PULLUP);
//
//  pinMode(Lim1Mot6, INPUT_PULLUP);
//  pinMode(Lim2Mot6, INPUT_PULLUP);
//
//  pinMode(Lim1NCV, INPUT_PULLUP);
//  pinMode(Lim2NCV, INPUT_PULLUP);
//  //*********************************************
//  //enable set up
//  pinMode(mot1Enab, OUTPUT);
//  pinMode(mot2Enab, OUTPUT);
//  pinMode(mot3Enab, OUTPUT);
//  pinMode(mot4Enab, OUTPUT);
//  pinMode(mot5Enab, OUTPUT);
//  pinMode(mot6Enab, OUTPUT);
//
//  //initalize all drivers to off:
//  digitalWrite(mot1Enab, LOW);
//  digitalWrite(mot2Enab, LOW);
//  digitalWrite(mot3Enab, LOW);
//  digitalWrite(mot4Enab, LOW);
//  digitalWrite(mot5Enab, LOW);
//  digitalWrite(mot6Enab, LOW);
//
//  // Set Driver Pins
//  N2OF.setMaxSpeed(450.0);
//  N2OF.setAcceleration(5000.0);
//  N2OF.setEnablePin(mot1Enab);
//
//  N2OV.setMaxSpeed(1000.0);
//  N2OV.setAcceleration(2000.0);
//  N2OV.setEnablePin(mot2Enab);
//
//  N2F.setMaxSpeed(450.0);
//  N2F.setAcceleration(5000.0);
//  N2F.setEnablePin(mot3Enab);
//
//  RTV.setMaxSpeed(400.0);
//  RTV.setAcceleration(5000.0);
//  RTV.setEnablePin(mot4Enab);
//
//  EVV.setMaxSpeed(240.0);
//  EVV.setAcceleration(5000.0);
//  EVV.setEnablePin(mot5Enab);
//
//  MEV.setMaxSpeed(280.0);
//  MEV.setAcceleration(5000.0);
//  MEV.setEnablePin(mot6Enab);
//
//
//
//  pinMode(Mot7, OUTPUT);
//  pinMode(Igniter, OUTPUT);
//  digitalWrite(Igniter, LOW);
//  //********************
//
//  //Serial_Startup
//  Serial.begin(9600);
//
//  //*********************************************
//
//  //Auto_Home(On Start up)
//  Serial.println("VCA,CF,Initalizing_to_safe_state:(Not_Implimented)");
//  //setState(1 ,1 ,1 ,1 ,1, 1);
//  digitalWrite(Mot7, LOW);
//
//  Serial.print("VCA,FD" );
//  Serial.print(",N2OF," + valve_N2OF.strState());
//  Serial.print(",N2OV," + valve_N2OV.strState());
//  Serial.print(",N2F," + valve_N2F.strState());
//  Serial.print(",RTV," + valve_RTV.strState());
//  Serial.print(",EVV," + valve_EVV.strState());
//  Serial.print(",MEV," + valve_MEV.strState());
//  Serial.print(",NCV," + valve_NCV.strState());
//  Serial.println("");
//
//  //*********************************************
//}
//
//
//
//void loop() {
//  Serial.flush();
//  //Finding Valve limit oriention: Test pouposes olnly
//  /*
//    Serial.println("Limit1: "+String(digitalRead(Lim1Mot1))+" Limit 2: "+String(digitalRead(Lim2Mot1)));
//    Serial.println("Limit1: "+String(digitalRead(Lim1Mot2))+" Limit 2: "+String(digitalRead(Lim2Mot2)));
//    Serial.println("Limit1: "+String(digitalRead(Lim1Mot3))+" Limit 2: "+String(digitalRead(Lim2Mot3)));
//    Serial.println("Limit1: "+String(digitalRead(Lim1Mot4))+" Limit 2: "+String(digitalRead(Lim2Mot4)));
//    Serial.println("Limit1: "+String(digitalRead(Lim1Mot5))+" Limit 2: "+String(digitalRead(Lim2Mot5)));
//    Serial.println("Limit1: "+String(digitalRead(Lim1Mot6))+" Limit 2: "+String(digitalRead(Lim2Mot6)));
//    Serial.println("Limit1: "+String(digitalRead(Lim1NCV))+" Limit 2: "+String(digitalRead(Lim2NCV)));
//  */
//
//  //*******************Waits for serial imput*******************
//  unsigned long Timeout = millis();
//
//  while (Serial.available() == 0) {
//    //Serial.println("Limit1: "+String(digitalRead(Lim1Mot2))+"Limit 2: "+String(digitalRead(Lim2Mot2)));
//    /*
//      //*******************Time-Out-Function****************
//      unsigned long Time = millis();
//      if(((Timeout + 15000)<Time) && ((Timeout + 30000)>Time)){
//        Serial.println("VCA,ER,NOCONECTION");
//      }
//      if(((Timeout + 30000)<Time)){
//        Serial.println("VCA,CD,ABORT");
//        digitalWrite(Igniter,LOW);
//        }
//
//    */
//  }
//
//  //****************I'm sure this is here for a reason**
//  char c = 'J';
//
//  //****************Read-Serial-Header*****************
//  String Buffer = Serial.readString();
//  String Command = "";
//  int cur = 0;
//
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//
//  Command = readSerial(cur, Buffer);
//  cur += Command.length() + 1;
//
//  //*******************ABORT-Comand*******************
//  if (Command.compareTo("ABORT") == 0) {
//    Serial.println("VCA,CF,ABORT");
//    digitalWrite(Igniter, LOW);
//
//    //fail state will be hard coded for safty purpoes
//
//    digitalWrite(Mot7, LOW);
//    setState(0, 0, 0, 0, 0, 0);
//
//
//    Serial.print("VCA,FD" );
//    Serial.print(",N2OF," + valve_N2OF.strState());
//    Serial.print(",N2OV," + valve_N2OV.strState());
//    Serial.print(",N2F," + valve_N2F.strState());
//    Serial.print(",RTV," + valve_RTV.strState());
//    Serial.print(",EVV," + valve_EVV.strState());
//    Serial.print(",MEV," + valve_MEV.strState());
//    Serial.print(",NCV," + valve_NCV.strState());
//    Serial.println("");
//
//    //*******************IGNITE-Comand*******************
//  } else if (Command.compareTo("IGNITE") == 0) {
//    digitalWrite(Igniter, HIGH);
//    Serial.println("VCA,CF,IGNITE");
//
//    //*******************VALVE-Comand*******************
//  } else if (Command.compareTo("CD") == 0) {
//    digitalWrite(Igniter, LOW);
//    Serial.println("VCA,CF,CD");
//    setSerial(cur, Buffer);
//
//    //*******************Test-Comands*******************
//  } else if (Command.compareTo("Open") == 0) {
//    //setSerial(); //make a function that reads searial and sets the requested state.
//
//    setState(-1, -1, -1, 0, -1, -1);
//    digitalWrite(Mot7, HIGH);
//
//
//  } else if (Command.compareTo("Close") == 0) {
//
//    setState(1, 1, 1, 0, 1, 1);
//    digitalWrite(Mot7, LOW);
//
//
//    //*******************Catch-all*******************
//  } else {
//    Serial.println("VCA,ER,Error:Incorect imput");
//  }
//}
