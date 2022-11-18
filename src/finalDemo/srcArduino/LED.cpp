
// LED.h
#ifndef LED_h
#define LED_h

#include <Arduino.h>

class LED {
  private:
    int ledPin;
    unsigned char ledState;

  public:
    LED(int pin);
    void turnON();
    void turnOFF();
    int getState();
};

#endif
// LED.cpp
#include "LED.h"

LED::LED(int pin) {
  ledPin   = pin;
  ledState = LOW;
  pinMode(ledPin, OUTPUT);
}

void LED::turnON() {
  ledState = HIGH;
  digitalWrite(ledPin, ledState);
}

void LED::turnOFF() {
  ledState = LOW;
  digitalWrite(ledPin, ledState);
}

int LED::getState() {
  return ledState;
}
