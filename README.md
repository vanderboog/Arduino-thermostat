# Thermostat

Code in this repository can be used to transform a Arduino (Uno) with additional components into a fully automatic thermostat, directly mounted to the turning knob of a radiator. The thermostat has four modes of operation:

1. Standby
2. Manual
3. Auto 1: workday
4. Auto 2: day off

Manual mode turns the heater knob to the same angle as the potentiometer is in. Whereas the auto modes use a PID control algorithm to regulate the temperature to the desired temperature. This desired temperature setpoint can be adjusted using the potentiometer knob. The use of PID control allows for a smooth operation of the thermostat in comparison to some cheap "smart radiator knobs". However, the PID control also makes it case specific in use. As the PID scheme can become unstable (or overshoot) in case of small spaces and slow in large spaces. The current settings are optimal in my case, which is a room of approximatedly 45-50 m^3. In general, small changes to the three parameters K_p, K_i and K_d should be sufficient for optimalization of other space. For more information about optimizing a PID controller I would like to refer to [wikipedia](https://en.wikipedia.org/wiki/PID_controller). 

The code features an internal clock. The clock keeps track of both time and day (without the use of a Real Time Clock (RTC) chip).  Time management is required for the thermostat to determine the temperature to regulation to. In addition, the LCD display backlight is turned off after some time of inactivity (i.e. no buttons pressed or turning of the knob). 

### Auto mode timers

Four timers are set in the code. These timers are required to determine the temperature to regulate to (either the setpoint or fixed value).  In the period inbetween the timers, the setpoint is used. In the intervals outside this timer, the temperature is regulated to a fixed temperature (unaltered, this is 12 **°**C). In Auto 2, regulation to the setpoint is used in the period of time between of timer[0] and timer[3]. For Auto 1 this is between the period of timer[0] to timer[1] and timer[2] to timer[3].

### Components

The components used in this setup:

- Arduino Uno
- I2C 16x2 Arduino LCD Display Module (1602A)
- Potentiometer
- Two push buttons
- MG996R 180° Digital Metal Gear Servo - 11kg/cm 

### Usage

The thermostat is intuitive to operate. There are two push buttons and a potentiometer to work with. All actions give feedback via the LCD display. The first button can be used to determine the mode in use. The second button can be used in manual and auto mode to set the time and day. Optionally, the timers can also be set in the menu with a small change in the code. Note that the settings are cleared when the Arduino is powered off. 

### Build

As for building the setup. I would like to leave this blank to challenge any new user to learn some more about the hardware. Do not forget to change the pins in the code if you decide to use different ones.