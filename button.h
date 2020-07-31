#ifndef MY_BUTTON_H
#define MY_BUTTON_H

class Button {

private:
  byte pin;
  byte state;
  byte newReading;
  byte lastReading;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 200;

public:
  Button(byte pin);

  void init();
  void update();

  byte getState();
  bool isPressed();
};

#endif

