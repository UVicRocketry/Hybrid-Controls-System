#ifndef MY_VALVE_H
#define MY_VALVE_H
#include <Arduino.h>
class Valve {
 
    private:
    int upperBound;
    int lowerBound;
    int change;
    int prevState;

    public:
    // Constructor
    Valve(int upperBound, int lowerBound);

    //State functions
    int state();
    String strState();

    // Getters
    int getUpperbound();
    int getlowerBound();
    int getChange();
};
#endif 
