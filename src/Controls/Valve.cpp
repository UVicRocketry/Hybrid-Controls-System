#include "Valve.h"

// Constructor
Valve::Valve(int upperBound, int lowerBound) {
  this->upperBound = upperBound;
  this->lowerBound = lowerBound;
  this->change = 0;
  this->prevState = 0;//!digitalRead(upperBound) | !digitalRead(lowerBound);
}

Valve::Valve(int upperBound, int lowerBound, int StepPin, int StepDir, int StepSpeed) {
  this->upperBound = upperBound;
  this->lowerBound = lowerBound;
  this->StepPin = StepPin;
  this->DirPin = DirPin;
  this->StepSpeed = StepSpeed;
  this->change = 0;
  this->prevState = 0;//!digitalRead(upperBound) | !digitalRead(lowerBound);
  pinMode(DirPin, OUTPUT);
  pinMode(StepPin, OUTPUT);
  StepTime = millis();
}

//*********State functions*********
int Valve::state() {
  if (digitalRead(upperBound) == LOW) {
    change = (prevState != -1);
    prevState = -1;
    return -1;
  } else if (digitalRead(lowerBound) == LOW) {
    change = (prevState != 1);
    prevState = 1;
    return 1;
  } else {
    change = (prevState != 0);
    prevState = 0;
    return 0;
  }
}

String Valve::strState() {
  if (digitalRead(upperBound) == LOW) {
    change = (prevState != -1);
    prevState = -1;
    return "OPEN";
  } else if (digitalRead(lowerBound) == LOW) {
    change = (prevState != 1);
    prevState = 1;
    return "CLOSE";
  } else {
    change = (prevState != 0);
    prevState = 0;
    return "TRANSIT";
  }
}


bool Valve::moveStep(int Dir)
{
  if ((millis() - StepTime) > StepSpeed)
  {
    digitalWrite(DirPin, Dir);
    digitalWrite(StepPin, 0);
    delayMicroseconds(5);
    digitalWrite(StepPin, 1);
    StepTime=millis();
    return true;
  }
  return false;
}
//*********Getters*********
int Valve::getUpperbound() {
  return this->upperBound;
}

int Valve::getlowerBound() {
  return this->lowerBound;
}


int Valve::getChange() {
  return change;
}
