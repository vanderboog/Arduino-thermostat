#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include "pidcontrol.h"

class Thermostat
{
  PidControl pid;

  ////////////////////////////////////////////
  // timer: set
  ////////////////////////////////////////////
  int timer[4];   //Set: wake up, to work, from work, and to sleep
  long clockTime; // Is long because of noDisplay() calculation
  long setLcdTimer;
  int setBackLightTimer;

  ////////////////////////////////////////////
  // Variable declaration
  ////////////////////////////////////////////
  int outPot;
  int outPotPrevious;
  double setPoint;
  double setPointNight;
  double outServo;
  double outputServo;
  int setTimeCounter;
  int mode;
  char strLine1[16];
  char floatVal[4];
  bool menuDisplay;

  double inTemp;
  String lcdText;


  int tempCounter = 0;
public:
  Thermostat();

  void menu();
  void modeSelection();
  void mode0();   // Standby
  void mode1();   // Manual
  void mode2_3(); // Automatic work-day/day-off
  void lcdDisplay();
};

#endif

