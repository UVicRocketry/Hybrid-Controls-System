#include "Arduino.h"
#include "Valve.h"

// Constructor
Valve::Valve(int OpenLimit, int CloseLimit) {
  this->OpenLimit = OpenLimit;
  this->CloseLimit = CloseLimit;
  this->change = 0;
  this->prevState = 0;//!digitalRead(OpenLimit) | !digitalRead(CloseLimit);
  pinMode(OpenLimit, INPUT_PULLUP);
  pinMode(CloseLimit, INPUT_PULLUP);
}

Valve::Valve(int OpenLimit, int CloseLimit, int StepPin, int DirPin, int StepSpeed) {
  this->OpenLimit = OpenLimit;
  this->CloseLimit = CloseLimit;
  this->StepPin = StepPin;
  this->DirPin = DirPin;
  this->StepSpeed = StepSpeed;
  this->change = 0;
  this->prevState = 0;//!digitalRead(OpenLimit) | !digitalRead(CloseLimit);
  pinMode(OpenLimit, INPUT_PULLUP);
  pinMode(CloseLimit, INPUT_PULLUP);
  pinMode(DirPin, OUTPUT);
  pinMode(StepPin, OUTPUT);
  StepTime = millis();
}

Valve::Valve(int OpenLimit, int CloseLimit, int SolEnab) {
  this->OpenLimit = OpenLimit;
  this->CloseLimit = CloseLimit;
  this->SolEnab = SolEnab;
  this->change = 0;
  this->activated = 1;
  this->prevState = 0;//!digitalRead(OpenLimit) | !digitalRead(CloseLimit);
  pinMode(OpenLimit, INPUT_PULLUP);
  pinMode(CloseLimit, INPUT_PULLUP);
  pinMode(SolEnab, OUTPUT);
  digitalWrite(SolEnab, LOW);  
}

//*********State functions*********
int Valve::state() {
  if (digitalRead(OpenLimit) == LOW) {
    change = (prevState != -1);
    prevState = -1;
    return -1;
  } else if (digitalRead(CloseLimit) == LOW) {
    change = (prevState != 1);
    prevState = 1;
    return 1;
  } else {
    change = (prevState != 0);
    prevState = 0;
    return 0;
  }
}

int Valve::solState() {
  if(activated == 1) {
    change = (prevState != 1);
    prevState = 1;
    return 1;
  } else if (activated == -1) {
    change = (prevState != -1);
    prevState = -1;
    return -1;
  } else {
    change = (prevState != 0);
    prevState = 0;
    return 0;
  }
}

String Valve::strState() {
  if (digitalRead(OpenLimit) == LOW) {
    change = (prevState != -1);
    prevState = -1;
    return "OPEN";
  } else if (digitalRead(CloseLimit) == LOW) {
    change = (prevState != 1);
    prevState = 1;
    return "CLOSE";
  } else {
    change = (prevState != 0);
    prevState = 0;
    return "TRANSIT";
  }
}

String Valve::solStrState() {
  if(activated == 1) {
    change = (prevState != 1);
    prevState = 1;
    return "CLOSE";
  } else if (activated == -1) {
    change = (prevState != -1);
    prevState = -1;
    return "OPEN";
  } else {
    change = (prevState != 0);
    prevState = 0;
    return "TRANSIT";    
  }
}

bool Valve::moveStep(int Dir)
{ 
  //Serial.print("Stepping Pin:");
  //Serial.println(StepPin);
  if ((millis() - StepTime) > StepSpeed)
  {
    //Serial.println(millis());
    //Serial.println(StepTime);
    //Serial.println(millis()-StepTime);
     if(Dir==1)
    {
       digitalWrite(DirPin, 1);
    }else
    {
      digitalWrite(DirPin, 0);
    }
    delayMicroseconds(50);
    digitalWrite(StepPin, 1);
    delayMicroseconds(50);
    digitalWrite(StepPin, 0);
    StepTime=millis();
    return true;
    
  }else if(StepTime>millis())
  {
    //overflow of 32bit millis number
    StepTime=millis();
  }
  return false;
}

bool Valve::moveSol(int State)
{
  if (State == 1)
  {
    digitalWrite(SolEnab, 0);
    activated = 1;
  } else
  {
    digitalWrite(SolEnab, 1);
    activated = -1;
  }
  return true;
}

//*********Getters*********
int Valve::getOpenLimit() {
  return this->OpenLimit;
}

int Valve::getCloseLimit() {
  return this->CloseLimit;
}


int Valve::getChange() {
  return change;
}
