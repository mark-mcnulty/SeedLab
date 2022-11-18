#ifndef robot_h
#define robot_h

#include <Arduino.h>
#include <Wire.h>
#include <Encoder.h>
#include <String.h>

class robot {
  private: 
    int encoderLeftOnePin;
    int encoderLeftTwoPin;
    int encoderRightOnePin;
    int encoderRightTwoPin;
    Encoder motorRight(encoderRightOnePin, encoderRightTwoPin);
    Encoder motorLeft(encoderLeftOnePin, encoderLeftTwoPin);

    unsigned char ledState;

  public:
    robot(int encoderLeftOnePinIn, int encoderLeftTwoPinIn, int );
};

#endif