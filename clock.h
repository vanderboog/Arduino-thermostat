#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h> // Library for LCD
#include "pins.h"
#include "button.h"

class Clock {
  // Internal clock variables. The internal clock implicitely assumes that the clock function is used every second!
  unsigned long timeNow;
  unsigned long timeLast;

  // Correct for inaccuracy of internal clock
  int dailyErrorFast;   // set the average number of milliseconds your microcontroller's time is fast on a daily basis
  int dailyErrorBehind; // set the average number of milliseconds your microcontroller's time is behind on a daily basis
  int correctedToday;   // one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.

  // Clock units
  int seconds;
  int minutes;
  int hours;
  int day;  // Holds number of day in a week

public:

  Clock();

  void update();
  short getClockTime();
  void setClockTime(Button& button2, LiquidCrystal_I2C& lcd);
  void setDayNumber(Button& button2, LiquidCrystal_I2C& lcd);
  short getDay();
  short getHours();
  short getMinutes();
  short getSeconds();
};

#endif

