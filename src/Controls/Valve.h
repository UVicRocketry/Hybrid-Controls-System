#ifndef MY_VALVE_H
#define MY_VALVE_H
#include <Arduino.h>
class Valve {
 
    private:
    int OpenLimit;
    int CloseLimit;
    int change;
    int prevState;
    uint32_t StepTime;
    int StepPin;
    int DirPin;
    int StepSpeed;

    public:
    // Constructor
    Valve(int OpenLimit, int CloseLimit);
    Valve(int OpenLimit, int CloseLimit, int StepPin, int DirPin, int StepSpeed);

    //State functions
    int state();
    String strState();
    bool moveStep(int Dir);

    // Getters
    int getOpenLimit();
    int getCloseLimit();
    int getChange();
};
#endif 
