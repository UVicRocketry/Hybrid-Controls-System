/*  Valve Cart and Feed System: Controls - Valve Controls - Full Asembly V 0.1
*  Author(s):
*  Logan Sewell
*  JJ
*  Organizarion: Uvic Rocketry
*/
#include "Valve.h" 
/* pins set for 1/32 micro stepping 
 *  Makes 200*32 pulses for making one full cycle rotation 
 *  Defines pins: 30 - 42 for Step==Dir Pairs
 *  Defines pins: 0 - 12 for Limit bounds
*/
//*********Limit Pins*********
const int Lim1Mot1 = 43;
const int Lim2Mot1 = 42;
const int Lim1Mot2 = 41;
const int Lim2Mot2 = 40;
const int Lim1Mot3 = 39;
const int Lim2Mot3 = 38;
const int Lim1Mot4 = 37;
const int Lim2Mot4 = 36;
const int Lim1Mot5 = 35;
const int Lim2Mot5 = 34;
const int Lim1Mot6 = 33;
const int Lim2Mot6 = 32;


const int Lim1NCV = 31;
const int Lim2NCV = 30;

//****************************

//*********Driver Pins********
const int StepMot1 = 4; 
const int DirMot1  = 3; 
const int StepMot2 = 6; 
const int DirMot2  = 5; 
const int StepMot3 = 8; 
const int DirMot3  = 7; 
const int StepMot4 = 9; 
const int DirMot4  = 10; 
const int StepMot5 = 12; 
const int DirMot5  = 11;
const int StepMot6 = 13; 
const int DirMot6  = 12; 

//solonoid and ignitor pin aclocation
const int Mot7 = 44;
const int Igniter = 45;
//********************

  //Assining mottors
  Motor Mot1 = {"Test 1", StepMot1, DirMot1, 60, 32};
  Motor Mot2 = {"Test 2", StepMot2, DirMot2, 60, 32};
  Motor Mot3 = {"Test 3", StepMot3, DirMot3, 60, 32};
  Motor Mot4 = {"Test 4", StepMot4, DirMot4, 60, 32};
  Motor Mot5 = {"Test 5", StepMot5, DirMot5, 60, 32};
  Motor Mot6 = {"Test 6", StepMot6, DirMot6, 60, 32};

  Motor VirtualMot7 ={"NCV", 0, 0, 0, 0};
 //*********************

 //Assining Valves (be sure to set proper oriention baied on mottor direction)
  Valve valve1 = Valve(Lim1Mot1, Lim2Mot1, 0, Mot1);
  Valve valve2 = Valve(Lim2Mot2, Lim1Mot2, 0, Mot2);
  Valve valve3 = Valve(Lim1Mot3, Lim2Mot3, 0, Mot3);
  Valve valve4 = Valve(Lim1Mot4, Lim2Mot4, 0, Mot4);
  Valve valve5 = Valve(Lim1Mot5, Lim2Mot5, 0, Mot5);
  Valve valve6 = Valve(Lim1Mot6, Lim2Mot6, 0, Mot6); 

  Valve valve7 = Valve(Lim1NCV, Lim2NCV, 0, VirtualMot7);
 
 //*********************

void setState(int s1, int s2, int s3, int s4, int s5, int s6){
 
  valve1.setDir(s1);
  valve2.setDir(s2);
  valve3.setDir(s3);
  valve4.setDir(s4);
  valve5.setDir(s5);
  valve6.setDir(s6);
  
  unsigned long temp = micros();
  //x is a time veriable used to regulate the step time of the mottors as they pulse
  int x;

  //n is the repetition veriable that detects if any error has ocurred
  long n = 0;
  
  while(n<100000 && (valve1.state()!= s1||valve2.state()!= s2||valve3.state()!= s3||valve4.state()!= s4||valve5.state()!= s5||valve6.state()!= s6)){
  n++;
  temp = micros();
  
  if(valve1.state()!= s1){
    valve1.pulse();
    }
    
  if(valve2.state()!= s2){
    valve2.pulse();
    }

  if(valve3.state()!= s3){
    valve3.pulse();
    }
    
  if(valve4.state()!= s4){
    valve4.pulse();
    }

  if(valve5.state()!= s5){
    valve5.pulse();
    }
    
  if(valve6.state()!= s6){
    valve6.pulse();
    }

    x = 70 -(micros() - temp);
    if(x <= 0){
      delayMicroseconds(1);
    }else{
      delayMicroseconds(x);
    }
  }
  
  if(valve1.state()!= s1||valve2.state()!= s2||valve3.state()!= s3||valve4.state()!= s4||valve5.state()!= s5||valve6.state()!= s6){
  Serial.print("VCA,CF,Error:,");
  if(valve1.state()!= s1){ Serial.print("Valve1," + valve1.strState()+ ","); }
  if(valve2.state()!= s2){ Serial.print("Valve2," + valve2.strState()+ ","); }
  if(valve3.state()!= s3){ Serial.print("Valve3," + valve3.strState()+ ","); }
  if(valve4.state()!= s4){ Serial.print("Valve4," + valve4.strState()+ ","); }
  if(valve5.state()!= s5){ Serial.print("Valve5," + valve5.strState()+ ","); }
  if(valve6.state()!= s6){ Serial.print("Valve6," + valve6.strState()); }
  Serial.println();
  /*
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot1))+" Limit 2: "+String(digitalRead(Lim2Mot1)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot2))+" Limit 2: "+String(digitalRead(Lim2Mot2)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot3))+" Limit 2: "+String(digitalRead(Lim2Mot3)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot4))+" Limit 2: "+String(digitalRead(Lim2Mot4)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot5))+" Limit 2: "+String(digitalRead(Lim2Mot5)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot6))+" Limit 2: "+String(digitalRead(Lim2Mot6)));
  Serial.println("Limit1: "+String(digitalRead(Lim1NCV))+" Limit 2: "+String(digitalRead(Lim2NCV)));
  */
  
  }
  }
  
  //***********************************************************
  
  //reades a String in serial comunication format and reads the next command
  String readSerial(int cur, String Buffer){
    if(cur > Buffer.length()){
      return "Error:_End_of_String";
    }
    String Command;
    char c;
    while(c != '\n' && c !=  ','){
      c = Buffer[cur]; 
      if(c != '\n' && c !=  ','){
        Command += c;
      }
      cur++;
    }
    
        return Command;
    }

  //*********************Set-Valves-to-serial-state***************************
 void setSerial(int cur, String Buffer){

  String Command;
   
    //************** Valve 1 *********************
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    int V1 = 2;
    if(Command.compareTo("OPEN") == 0){
      V1 = 0;
      }else if (Command.compareTo("CLOSE") == 0){
      V1 = 1; 
      }else{
         Serial.println("Error Incorect imput 1: [" + Command + "]");
        }
   
    //************** Valve 2 *********************
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    int V2 = 2;
    if(Command.compareTo("OPEN") == 0){
      V2 = 0;
     }else if (Command.compareTo("CLOSE") == 0){
      V2 = 1; 
      }else{
         Serial.println("Error Incorect imput 2: [" + Command + "]");
      }

    //************** Valve 3 *********************
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    int V3 = 2;
    if(Command.compareTo("OPEN") == 0){
      V3 = 0;
      }else if (Command.compareTo("CLOSE") == 0){
      V3 = 1; 
      }else{
       Serial.println("Error Incorect imput 3: [" + Command + "]");
      }
      
      //************** Valve 4 *********************
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    int V4 = 2;
    if(Command.compareTo("OPEN") == 0){
      V4 = 0;
      }else if (Command.compareTo("CLOSE") == 0){
        V4 = 1; 
      }else{
        Serial.println("Error Incorect imput 4: [" + Command + "]");
      }

    //************** Valve 5 *********************
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    int V5 = 2;
    if(Command.compareTo("OPEN") == 0){
      V5 = 0;
      }else if (Command.compareTo("CLOSE") == 0){
      V5 = 1; 
      }else{
       Serial.println("Error Incorect imput 5: [" + Command + "]");
      }
      
      //************** Valve 6 *********************
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    int V6 = 2;
    if(Command.compareTo("OPEN") == 0){
      V6 = 0;
      }else if (Command.compareTo("CLOSE") == 0){
      V6 = 1; 
      }else{
       Serial.println("Error Incorect imput 6: [" + Command + "]");
      }

      //************** Valve 7 *********************
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    
    Command = readSerial(cur, Buffer);
    cur += Command.length() + 1;
    int V7 = 2;
    if(Command.compareTo("OPEN") == 0){
      V7 = 0;
      digitalWrite(Mot7,HIGH);
      }else if (Command.compareTo("CLOSE") == 0){
      V7 = 1;
      digitalWrite(Mot7,LOW);
      }else{
       Serial.println("Error Incorect imput 6: [" + Command + "]");
      }
      
      setState(V1, V2, V3, V4, V5, V6);
      Serial.println("It worked!!" );
     
      Serial.print("VCA,CF" );
      Serial.print(",V1" + valve1.strState());
      Serial.print(",V2" + valve2.strState());
      Serial.print(",V3" + valve3.strState());
      Serial.print(",V4" + valve4.strState());
      Serial.print(",V5" + valve4.strState());
      Serial.print(",V5" + valve6.strState());
      Serial.print(",V7" + valve7.strState());
      Serial.println("");
  }
  
    
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
 
 // Set Driver Pins
  pinMode(StepMot1,OUTPUT); 
  pinMode(DirMot1,OUTPUT);
  digitalWrite(DirMot1,HIGH);
  
  pinMode(StepMot2,OUTPUT); 
  pinMode(DirMot2,OUTPUT);
  digitalWrite(DirMot2,HIGH);
  
  pinMode(StepMot3,OUTPUT); 
  pinMode(DirMot3,OUTPUT);
  digitalWrite(DirMot3,HIGH);
  
  pinMode(StepMot4,OUTPUT); 
  pinMode(DirMot4,OUTPUT);
  digitalWrite(DirMot4,HIGH);

  pinMode(StepMot5,OUTPUT); 
  pinMode(DirMot5,OUTPUT);
  digitalWrite(DirMot5,HIGH);

  pinMode(StepMot6,OUTPUT); 
  pinMode(DirMot6,OUTPUT);
  digitalWrite(DirMot6,HIGH);


  pinMode(Mot7,OUTPUT);
  
  pinMode(Igniter,OUTPUT); 
  digitalWrite(Igniter,LOW);
  //********************
  
  //Serial_Startup
  Serial.begin(9600);
  
 //*********************************************
 
 //Auto_Home(On Start up)
 Serial.println("VCA,CF,Initalizing_to_safe_state:(Not_Implimented)");
  /* un-comment to implement Auto_Home
  setState(2 ,2 ,2 ,2 ,2, 2);
  digitalWrite(Mot7,LOW);

  Serial.print("VCA,CF" );
  Serial.print(",V1" + valve1.strState());
  Serial.print(",V2" + valve2.strState());
  Serial.print(",V3" + valve3.strState());
  Serial.print(",V4" + valve4.strState());
  Serial.print(",V5" + valve4.strState());
  Serial.print(",V5" + valve6.strState());
  Serial.print(",V7" + valve7.strState());
  Serial.println("");
  */
 //*********************************************
}


 
void loop() {
 
 //Finding Valve limit oriention: Test pouposes olnly
 
  /*  
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot1))+" Limit 2: "+String(digitalRead(Lim2Mot1)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot2))+" Limit 2: "+String(digitalRead(Lim2Mot2)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot3))+" Limit 2: "+String(digitalRead(Lim2Mot3)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot4))+" Limit 2: "+String(digitalRead(Lim2Mot4)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot5))+" Limit 2: "+String(digitalRead(Lim2Mot5)));
  Serial.println("Limit1: "+String(digitalRead(Lim1Mot6))+" Limit 2: "+String(digitalRead(Lim2Mot6)));
  Serial.println("Limit1: "+String(digitalRead(Lim1NCV))+" Limit 2: "+String(digitalRead(Lim2NCV)));
  */
  
 //*******************Waits for serial imput*******************
  unsigned long Timeout = millis();
  
 while (Serial.available() == 0){
   //Serial.println("Limit1: "+String(digitalRead(Lim1Mot2))+"Limit 2: "+String(digitalRead(Lim2Mot2)));

 /*
  //*******************Time-Out-Function****************
  unsigned long Time = millis();
  if(((Timeout + 15000)<Time) && ((Timeout + 30000)>Time)){
    Serial.println("VCA,NOCONECTION");
  }
  if(((Timeout + 30000)<Time)){
    Serial.println("VCA,ABORT");
    digitalWrite(Igniter,LOW);
    }
    */
  }
  
 //****************I'm sure this is here for a reason**
 char c = 'J';

 //****************Read-Serial-Header*****************
 String Buffer = Serial.readString();
 String Command = "";
 int cur = 0;

  Command = readSerial(cur, Buffer);
  cur += Command.length() + 1;
  
  Command = readSerial(cur, Buffer);
  cur += Command.length() + 1;

  //*******************ABORT-Comand*******************
  if(Command.compareTo("ABORT") == 0){
     Serial.println("VCA,CF,ABORT");
     Serial.println("(Not Implimented)");
     digitalWrite(Igniter,LOW);
     
     //fail state will be hard coded for safty purpoes
     //setState(1, 1, 2, 2, 2, 2);
     digitalWrite(Mot7,LOW);
     Serial.println("Emergency state reached:");

   //*******************IGNITE-Comand*******************
   }else if(Command.compareTo("IGNITE") == 0){
     digitalWrite(Igniter,HIGH);
     Serial.println("VCA,CF,IGNITE"); 
    
   //*******************VALVE-Comand*******************
   }else if(Command.compareTo("CD") == 0) {
    digitalWrite(Igniter,LOW);
    Serial.println("VCA,CF,CD");
      setSerial(cur, Buffer);

   //*******************Test-Comands*******************
   }else if(Command.compareTo("Open") == 0){
    //setSerial(); //make a function that reads searial and sets the requested state.
    setState(0, 0, 0, 2, 2, 2);
    digitalWrite(Mot7,HIGH);
    
   
   }else if(Command.compareTo("Close") == 0){
    setState(1, 1, 1, 2, 2, 2);
    digitalWrite(Mot7,LOW);
    

    //*******************Catch-all******************* 
   }else{
    Serial.println("VCA,CF,Error:Incorect imput");
   }
      
 }
