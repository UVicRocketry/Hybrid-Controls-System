#ifndef MY_VALVE_H
#define MY_VALVE_H
#include <Arduino.h>
class Valve {
 
    private:
    int upperBound;
    int lowerBound;



    public:
    // Constructor
    Valve(int upperBound, int lowerBound);

    //State functions
    int state();
    String strState();

    // Getters
    int getUpperbound();
    int getlowerBound();
};
#endif 
