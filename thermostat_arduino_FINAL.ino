#include <Arduino.h>
#include <Servo.h>             // Library for servo control
#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <OneWire.h>           // Required for temperature sensor
#include <DallasTemperature.h> // Required for temperature sensor
#include "thermostat.h"
#include "objects.h"
#include "pins.h"
#include "clock.h"
#include "button.h"


/// Pins
const byte POTENTIO_PIN = 2;
const byte TEMP_SENSOR_PIN = 3;
const byte BUTTON_1_PIN = 4;
const byte BUTTON_2_PIN = 5;
const byte SERVO_PIN = 9;

// Print results on serial:
const bool serialPrint = 1;

// Objects
Servo servo;
Clock clock;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2); // Use (0x3F,16,2) for 16x2 LCD.
Button button1(BUTTON_1_PIN);
Button button2(BUTTON_2_PIN);
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature sensorTemp(&oneWire);

// Create thermostat after objects
Thermostat thermostat;

void setup()
{
  // Set serial communication for temperature sensor and monitor
  Serial.begin(9600);
  
  // Setup objects
  lcd.init();
  lcd.clear();
  servo.attach(SERVO_PIN);
  sensorTemp.begin();

  if(serialPrint){
    Serial.print("P = ");
    //Serial.print(k_p);
    Serial.print("\tI = ");
    //Serial.print(k_i);
    Serial.print("\tD = ");
    //Serial.print(k_d);
    Serial.print("\n P, I, D, PID output, ServoAngle, dTime, Temperature,  Clock\n");
  }
}

void loop()
{
  thermostat.modeSelection();
}






