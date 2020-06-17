#ifndef OBJECTS_H
#define OBJECTS_H

#include <LiquidCrystal_I2C.h> // Library for LCD
#include <OneWire.h>           // Required for temperature sensor
#include <DallasTemperature.h> // Required for temperature sensor
#include <Servo.h>
#include "clock.h"
#include "button.h"

extern Servo servo;
extern Clock clock;
extern LiquidCrystal_I2C lcd;
extern Button button1;
extern Button button2;
extern OneWire oneWire;
extern DallasTemperature sensorTemp;

#endif
