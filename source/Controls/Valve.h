#ifndef MY_VALVE_H
#define MY_VALVE_H
#include <Arduino.h>

struct Motor{
    String Location;
    int stepPin;
    int dirPin;
    int motSpeed;
    int stepMult;
    };

class Valve {
 
    private:

    Motor motorOne;
    int upperBound;
    int lowerBound;
    int orientation;


    public:

    // Constructor
    Valve(int upperBound, int lowerBound, int orientation, Motor motorOne);

    //valve Function (out of date)
    /*
    void turn(double Fraction, int dir, int Speed);
    bool Open();
    bool Close();
    */

    //State functions
    int state();
    String strState();
    bool isOpen();
    void pulse();
    void setDir(int dir);

    // Getters
    int getUpperbound();
    int getlowerBound();
    Motor getMotor();
};
#endif 
