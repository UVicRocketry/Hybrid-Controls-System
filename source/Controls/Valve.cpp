#include "Valve.h" 

    // Constructor
    Valve::Valve(int upperBound, int lowerBound, int orientation, Motor motorOne){
    this->upperBound = upperBound;
    this->lowerBound = lowerBound;
    this->motorOne = motorOne;
    this->orientation = orientation;
    }
 /*
    void Valve::turn(double Fraction, int dir, int Speed){
      if((dir - orientation) == 0){
        digitalWrite(motorOne.dirPin,HIGH);
      }else{
        digitalWrite(motorOne.dirPin,LOW);
      }
      for(int i = 0; i < (200 * motorOne.stepMult * Fraction); i++){
        digitalWrite(motorOne.stepPin,HIGH); 
        delayMicroseconds(5);
        digitalWrite(motorOne.stepPin,LOW);
        //delayMicroseconds(10);
        delayMicroseconds(Speed);
      }
    }
    

    bool Valve::Open(){
      int n = 0;
      while(digitalRead(lowerBound) != LOW){
        turn(0.125, 1,motorOne.motSpeed);
        n++;
        if(n >=100){
          return false;
          digitalWrite(motorOne.stepPin,LOW);
        }
      }
      return true;
      digitalWrite(motorOne.stepPin,LOW);
    }

    bool Valve::Close(){
      int n = 0;
      while(digitalRead(upperBound) != LOW){
        turn(0.125, 0,motorOne.motSpeed);
        n++;
        if(n >= 100){
          return false;
          digitalWrite(motorOne.stepPin,LOW);
        }
      }
      return true;
      digitalWrite(motorOne.stepPin,LOW);
    }
    */

    //*********State functions*********
    int Valve::state(){
      if(digitalRead(upperBound) == LOW){
          return 0;
        }else if(digitalRead(lowerBound) == LOW){
          return 1;
        }else{
          return 2;
          }
      }


    void Valve::pulse(){
      digitalWrite(motorOne.stepPin,HIGH); 
      delayMicroseconds(5);
      digitalWrite(motorOne.stepPin,LOW);
      }
      

    void Valve::setDir(int dir){
      if((dir - orientation) == 0){
        digitalWrite(motorOne.dirPin,HIGH);
      }else{
        digitalWrite(motorOne.dirPin,LOW);
      }
      }

      

    String Valve::strState(){
      if(digitalRead(upperBound) == LOW){
        return "Open";
      }else if(digitalRead(lowerBound) == LOW){
        return "Closed";
      }else{
        return "Unknown";
        }
      }

    //*********Getters*********
    int Valve::getUpperbound(){
    return this->upperBound;
    }

    int Valve::getlowerBound(){
    return this->lowerBound;     
    }

    Motor Valve::getMotor(){
    return this->motorOne;
    }
