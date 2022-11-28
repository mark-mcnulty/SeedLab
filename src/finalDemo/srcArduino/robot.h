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
    bool turn(float markerAngleRad, float time_start, float time_end);
    bool drive(float desiredTheta, float time_start, float time_past);
};

#endif