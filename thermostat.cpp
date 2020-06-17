#include "thermostat.h"

Thermostat::Thermostat() : 
timer({
  700, 800, 1630, 2358}
),
setLcdTimer(-1),
setBackLightTimer(30), //Set time before shutting off LCD setBackLightTimer
setTimeCounter(0),
setPointNight(12.0), // Setpoint to regulate to in absence or at night ( in degrees Celsius)
mode(0),
menuDisplay(false)
{
}

// Menu which allows to set hours and minutes of: clock, timer[0], timer[1], timer[2], timer[3] (changes to timers require a change of digit below)
void Thermostat::menu()
{
  int counter = 0;
  int setHoursTimer;
  int setMinutesTimer;
  menuDisplay = true;
  lcd.backlight();
  lcd.blink();
  clock.setClockTime(button2, lcd);
  clock.setDayNumber(button2, lcd);

  while (counter < 0) // Change from 0 to 4 to be able to set timers
  {
    while (!button2.isPressed())
    {
      outPot = analogRead(POTENTIO_PIN);
      outPot = map(outPot, 0, 1023, 0, 287); // 24*(60/5)-1=287 allows to set time per 5 minutes()

      if (outPotPrevious != outPot) {
        setHoursTimer = outPot / 12;
        setMinutesTimer = outPot % 12 * 5;
      }

      // Set timers to start/stop
      if (counter == 0)
      {
        timer[0] = setHoursTimer * 100 + setMinutesTimer;
        sprintf(strLine1, "Opstaan:  %02u:%02u", setHoursTimer, setMinutesTimer);
      }
      else if (counter == 1)
      {
        timer[1] = setHoursTimer * 100 + setMinutesTimer;
        sprintf(strLine1, "Vertrek:  %02u:%02u", setHoursTimer, setMinutesTimer);
      }
      else if (counter == 2)
      {
        timer[2] = setHoursTimer * 100 + setMinutesTimer;
        sprintf(strLine1, "Aankomst  %02u:%02u", setHoursTimer, setMinutesTimer);
      }
      else if (counter == 3)
      {
        timer[3] = setHoursTimer * 100 + setMinutesTimer;
        sprintf(strLine1, "Nacht:    %02u:%02u", setHoursTimer, setMinutesTimer);
      }
      lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
      lcd.print(strLine1);
      outPotPrevious == outPot;
    }
    counter += 1;
  }
  lcd.noBlink();
  clockTime = clock.getHours() * 100 + clock.getMinutes();
  outPotPrevious = outPot;
}

void Thermostat::modeSelection()
{
  // Each mode traps the program. Upon release, the next mode is active
  if (mode == 0)
  {
    mode0();
  }
  else if (mode == 1)
  {
    mode1();
  }
  else if (mode == 2 || mode == 3)
  {
    mode2_3();
  }
  mode = (mode+1) % 4;
}

// Standby mode:
void Thermostat::mode0()
{
  servo.write(179); // Set the value to standby angle
  lcd.clear();
  lcd.noBacklight();
  while (!button1.isPressed()) // Continue loop until button press for mode increment
  {
    clock.update();
    if (clock.getClockTime() >= 2358)
    {
      setLcdTimer = -1;
    }
  }
  lcd.backlight();
  setLcdTimer = -1;
  outServo = 0; // To reset angle of servo in auto when circling through modes
}

// Manual mode: Potentiometer directly 
void Thermostat::mode1()
{
  while (!button1.isPressed()) // Trap program in this mode untill button press
  {
    outPot = analogRead(POTENTIO_PIN);
    outPot = map(outPot, 0, 1023, 180, 0);

    if (button1.isPressed()) { 
      break; 
    } // Button does not respond if code takes to long

    if (outPot < 5)
    {
      outPot = 180;
    } // Failsafe: if cable is disconnected, the output cannot go to maximum!

    if (outPot != outPotPrevious)
    {
      servo.write(outPot);
    }

    if (button2.isPressed())
    {
      menu();
    }

    if (button1.isPressed()) { 
      break; 
    } // Button does not respond if code takes to long

    sensorTemp.requestTemperatures();
    inTemp = sensorTemp.getTempCByIndex(0);

    if (button1.isPressed()) { 
      break; 
    } // Button does not respond if code takes to long

    lcdDisplay();
  }
}

void Thermostat::mode2_3()
{
  // Turn on LCD if mode changed (automatically turned off after timer)
  clock.update();
  clockTime = clock.getClockTime();
  setLcdTimer = (long)(clock.getClockTime() + (clock.getSeconds() + setBackLightTimer) / 60) * 100 + (clock.getSeconds() + setBackLightTimer) % 60; // Calculation more difficult due to clock.getSeconds() to minute conversion

  while (!button1.isPressed()) // Trap program in this mode untill button press
  {
    // Read and rescale potentiometer
    outPot = analogRead(POTENTIO_PIN);
    outPot = map(outPot, 0, 1023, 30, 50);

    if (outPot > 48) // Failsafe: If one a cable is detached, the output cannot go to maximum value
    {
      outPot = 40; //Since it is divided by two it defaults to 20 degrees celsius when disconnected
    } 

    // Change temperature setPoint if potentiometer is turned
    if (outPot != outPotPrevious)
    {
      setPoint = ((double) outPot) / 2;
    }

    // Thermostat acts only once per minute
    clock.update();
    if(clock.getClockTime() < 1) { // After clocktime 2359, it needs to be reset to zero 
      clockTime = 0;
    }
    else if (clock.getClockTime() > clockTime)
    {
      // Read temperature;
      sensorTemp.requestTemperatures();
      inTemp = (double)sensorTemp.getTempCByIndex(0);

      // Update clocktime
      clockTime = clock.getClockTime();

      if (clockTime < timer[0])
      {
        outServo = outServo + pid.compute(inTemp, setPointNight);
      }
      else if (clockTime > timer[3])
      {
        outServo = outServo + pid.compute(inTemp, setPointNight);
      }
      else if (mode == 2)
      {
        if (clock.getDay() % 7 > 4)
        {
          outServo = outServo + pid.compute(inTemp, setPoint);
        }
        else if ((clockTime > timer[1]) && (clockTime < timer[2]))
        {
          outServo = outServo + pid.compute(inTemp, setPointNight);
        }
        else if (clockTime == timer[0] || clockTime == timer[2])
        {
          outServo = 0;
        } // Makes sure outServo is reset before turning on
        else
        {
          outServo = outServo + pid.compute(inTemp, setPoint);
        }
      }
      else if (mode == 3)
      {
        if (clockTime == timer[0])
        {
          outServo = 0;
        } // Makes sure outServo is reset before turning on
        else
        {
          outServo = outServo + pid.compute(inTemp, setPoint);
        }
      }

      // Limit the servo value. Without it, the value would exceed the limit, which will require time to get back from!
      if (outServo < 0.0)
      {
        outServo = 3.0;
      }
      if (outServo > 180.0)
      {
        outServo = 177.0;
      } // Can be set to max (180 degrees) minus some margin

      if (outServo == outServo)
      { // Prevents passing of NaN (failsafety)
        // Set new servoangle
        outputServo = map(outServo, 0.0, 180.0, 180.0, 0.0); // For failsafety!
        servo.write(outputServo);
        if(serialPrint){
          Serial.print(outputServo);
        }
      }
      else
      {
        outServo = 0.0;
        if(serialPrint){
          Serial.print("NaN");
        }
      }

      // Print results to serial
      if(serialPrint){
        Serial.print(",\t");
        Serial.print(inTemp);
        Serial.print(",\t");
        Serial.print(clockTime);
        Serial.print("\t");
        Serial.print(setPoint);
        Serial.print("\n");
      }
    }

    if (button1.isPressed()) { 
      break; 
    } // Button does not respond if code takes to long

    // Enter menu on button2 press
    if (button2.isPressed())
    {
      menu();
    }

    lcdDisplay();
  }
}

void Thermostat::lcdDisplay()
{
  // Put lcd backlight
  if (button1.isPressed() || button2.isPressed() || outPotPrevious != outPot)
  {
    lcd.backlight();
    clockTime = clock.getClockTime();
    setLcdTimer = (long)(clockTime + (clock.getSeconds() + setBackLightTimer) / 60) * 100 + (clock.getSeconds() + setBackLightTimer) % 60; // Calculation more difficult due to clock.getSeconds() to minute conversion
  }

  // Display info if LCD timer is not expired
  if (setLcdTimer > (long)(clockTime * 100 + clock.getSeconds()))
  {
    // Set time to string for visualisation
    if (mode == 2)
    {
      sprintf(strLine1, "Auto1     %02u:%02u", clock.getHours(), clock.getMinutes());
      lcd.setCursor(0, 1); //Set the cursor on the second column and the second row (counting starts at 0!).
      lcd.print((double)setPoint, 1);
      lcd.print("      ");
    }
    else if (mode == 3)
    {
      sprintf(strLine1, "Auto2     %02u:%02u", clock.getHours(), clock.getMinutes());
      lcd.setCursor(0, 1); //Set the cursor on the second column and the second row (counting starts at 0!).
      lcd.print((double)setPoint, 1);
      lcd.print("      ");
    }
    else if (mode == 1)
    {
      //sprintf (strLine1, "Manual    %02u:%02u", clock.getHours(), clock.getMinutes());
      lcd.setCursor(0, 1); //Set the cursor on the second column and the second row (counting starts at 0!).
      sprintf(strLine1, "%03u       ", 180 - outPot);
      lcd.print(strLine1);
      sprintf(strLine1, "Manual    %02u:%02u", clock.getHours(), clock.getMinutes());
    }

    // Display generic info (temperature)
    lcd.print(inTemp, 1);
    lcd.print((char)223); //degrees symbol
    lcd.print("C");

    // Print on LCD
    lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
    lcd.print(strLine1);
  }
  else
  {
    lcd.clear();
    lcd.noBacklight();
  }

  // Reset setLcdTimer so that backlight will be off after going round the clock
  if (clockTime >= 2358)
  {
    setLcdTimer = -1;
  }
}








