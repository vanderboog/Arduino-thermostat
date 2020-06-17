#ifndef PIDCONTROL_H
#define PIDCONTROL_H

#include <Arduino.h>
#include "pins.h"

class PidControl{
    ///////////////////////////////////////////
    ///PID control variables
    ///////////////////////////////////////////
    // P:0.9, I:0.2, D:1.6 (works, yet slow in ms)
    // P:1.0, I:0.3, D:3.0 (works, set per min)
    // PID gain parameters ( 0.01, 0.1, 0.1 works fine in continues mode)
    double k_p; // 0.9 //0.3  ,0.01  // 0.01 turns ~10 deg per min
    double k_i;  //0.2//.003;//0.003;    //0.9 ,0.003
    double k_d;  //1.6 //0.2//1;    //0.6     ,75

    //PID Variables
    double input, output, setPoint;
    unsigned long currentTime, previousTime;
    double dTime;
    double error, previousError;
    double intError;
    double dError;
    
    public:
    PidControl();
    
    double compute(double in, double set);

};

#endif
