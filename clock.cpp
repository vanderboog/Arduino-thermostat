#include <Arduino.h>
#include "clock.h"
#include "pins.h"

Clock::Clock() : 
timeNow(0),
timeLast(0),
// Correct for inaccuracy of internal clock
dailyErrorFast(0), // set the average number of milliseconds your microcontroller's time is fast on a daily basis
dailyErrorBehind(0), // set the average number of milliseconds your microcontroller's time is behind on a daily basis
correctedToday(1), // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.
// Initial time of clock:
seconds(0),
minutes(0),
hours(12),
day(0)
{}

void Clock::update()
{
  timeNow = millis() / 1000;    // the number of milliseconds that have passed since boot
  seconds = timeNow - timeLast; //the number of seconds that have passed since the last time 60 seconds was reached.

  //if one minute has passed, start counting seconds from zero again and add one minute to the clock.
  if (seconds >= 60)
  {
    timeLast = timeNow - seconds % 60; //Substract (seconds % 60) in case seconds > 60 (corrects clock in case cycletime > 1 second)
    minutes = minutes + seconds / 60; // (Seconds / 60) (usually 1) is required in case time corrections of > 60 seconds are applied 
  }

  // if one hour has passed, start counting minutes from zero and add one hour to the clock
  if (minutes >= 60)
  {
    minutes = minutes % 60;
    hours = hours + 1;
  }

  //if 24 hours have passed , add one day
  if (hours >= 24)
  {
    hours = hours % 24;
    day = (day + 1)%7;
  }

  //Every 24 hours at 2:00 and while not already reset this day, add milliseconds or delay the progran with some milliseconds
  if (hours == 2 && correctedToday == 0)
  {
    delay(dailyErrorFast * 1000);
    timeLast = timeLast - dailyErrorBehind; // implicit correction to variable: seconds
    correctedToday = 1;
  }

  // Set correctedToday to 0 to correct again next day 
  if (hours == 3) 
  {
    correctedToday = 0;
  } 
}

short Clock::getClockTime(){
  return hours * 100 + minutes;
}

void Clock::setClockTime(Button& button2, LiquidCrystal_I2C& lcd){
  short outPot;
  char strLine1[16];
  while (!button2.isPressed())
  {
    outPot = analogRead(POTENTIO_PIN);
    outPot = map(outPot, 0, 1023, 0, 287);
    hours = outPot / 12;
    sprintf(strLine1, "Klok:     %02u:%02u",  hours, minutes);
    lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
    lcd.print(strLine1);
  }

  while (!button2.isPressed())
  {
    outPot = analogRead(POTENTIO_PIN);
    outPot = map(outPot, 0, 1023, 0, 59);
    minutes = outPot;
    sprintf(strLine1, "Klok:     %02u:%02u", hours, minutes);
    lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
    lcd.print(strLine1); 
  }

}

void Clock::setDayNumber(Button& button2, LiquidCrystal_I2C& lcd){
  short outPot;
  char strLine1[16];
  String dayName;
  while(!button2.isPressed()) 
  {
    outPot = analogRead(POTENTIO_PIN);
    day = map(outPot, 0, 1023, 0, 6);
    if (day == 0)
    {
      dayName = "Maandag         ";
    }
    if (day == 1)
    {
      dayName = "Dinsdag         ";
    }
    if (day == 2)
    {
      dayName = "Woensdag        ";
    }
    if (day == 3)
    {
      dayName = "Donderdag       ";
    }
    if (day == 4)
    {
      dayName = "Vrijdag         ";
    }
    if (day == 5)
    {
      dayName = "Zaterdag        ";
    }
    if (day == 6)
    {
      dayName = "Zondag          ";
    }

    sprintf(strLine1, "Dag:      %02u:%02u", hours, minutes);
    lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
    lcd.print(strLine1);
    lcd.setCursor(0, 1); // Set the cursor on the first column and second row.
    lcd.print(dayName); 
  }
}

short Clock::getDay(){
  return day;
}

short Clock::getHours(){
  return hours;
}

short Clock::getMinutes(){
  return minutes;
}

short Clock::getSeconds(){
  return seconds;
}







