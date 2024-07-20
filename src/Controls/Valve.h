#ifndef MY_VALVE_H
#define MY_VALVE_H
#include <Arduino.h>
class Valve {
 
    private:
    int OpenLimit;
    int CloseLimit;
    int SolEnab;
    int change;
    int prevState;
    uint32_t StepTime;
    int StepPin;
    int DirPin;
    int StepSpeed;
    int activated;

    public:
    // Constructor
    Valve(int OpenLimit, int CloseLimit);
    Valve(int OpenLimit, int CloseLimit, int StepPin, int DirPin, int StepSpeed);
    Valve(int OpenLimit, int CloseLimit, int SolEnab);

    //State functions
    int state();
    int solState();
    String strState();
    String solStrState();
    bool moveStep(int Dir);
    bool moveSol(int State);

    // Getters
    int getOpenLimit();
    int getCloseLimit();
    int getChange();
};
#endif 
