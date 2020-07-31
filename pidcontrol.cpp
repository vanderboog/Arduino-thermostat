#include <Arduino.h>
#include "pidcontrol.h"
#include "pins.h"

PidControl::PidControl() : 
k_p(1.0),
k_i(0.3),
k_d(3.0)
{}

double PidControl::compute(double in, double set)
{
  currentTime = millis();
  dTime = ((double)currentTime - previousTime) / 1000;

  if (dTime > 0.7)
  { // Failsafe: PID is unstable for small dTime (should not occur anyway)

    // Deterime Proportional, Integral and Differential parts respectively
    error = set - in;
    intError += error * dTime;
    dError = (error - previousError) / dTime;
  }

  // Compute output value
  double out = k_p * error + k_i * intError + k_d * dError;

  if(serialPrint){
    Serial.print(error);
    Serial.print(",\t");
    Serial.print(intError);
    Serial.print(",\t");
    Serial.print(dError);
    Serial.print(",\t");
    Serial.print(out);
    Serial.print(",\t");
  }

  // Store error and time for next iteration
  previousError = error;
  previousTime = currentTime;

  return out;
}

