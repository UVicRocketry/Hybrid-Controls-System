#ifndef MY_VALVE_H
#define MY_VALVE_H
#include <Arduino.h>
class Valve {
 
    private:
    int upperBound;
    int lowerBound;
    int change;
    int prevState;
    int StepTime;
    int StepPin;
    int DirPin;
    int StepSpeed;

    public:
    // Constructor
    Valve(int upperBound, int lowerBound);
    Valve(int upperBound, int lowerBound, int StepPin, int DirPin, int StepSpeed);

    //State functions
    int state();
    String strState();
    bool moveStep(int Dir);

    // Getters
    int getUpperbound();
    int getlowerBound();
    int getChange();
};
#endif 
