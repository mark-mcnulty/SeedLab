#include <Encoder.h>
#define CLK_L_PIN 3
#define CLK_R_PIN 2
#define DT_L_PIN 6
#define DT_R_PIN 7

#define COUNTS_PER_ROTATION 128

volatile int Direction = 0;

float left = 0.0;
float left_AV = 0.0;
float leftVelocity = 0.0;

float right = 0.0;
float right_AV = 0.0;
float rightVelocity = 0.0;

float TIME = 0.0;
float dif_time_L = 0.0;

//In centimeters Not converted
float r = 7.6;
int count = 0;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(CLK_L_PIN), CLK_L_ISR, CHANGE);  // change rising or falling
  attachInterrupt(digitalPinToInterrupt(CLK_R_PIN), CLK_R_ISR, CHANGE);  // pin, function, flag to look for
}


void loop() {
  if (Direction == 1 && abs(leftVelocity) < 2 && abs(rightVelocity) < 2) {
    //Serial.print(count);
    //Serial.print("\t");
    Serial.print(TIME / 1000);
    Serial.print("\t");
    Serial.print(leftVelocity);
    Serial.print("\t");
    Serial.println(rightVelocity);
    Direction = 0;
  }
  delay(100);
}

void CLK_L_ISR() {
  // assign the time
  dif_time_L = millis() - TIME;
  // assign the time

  // find the angular position of the encoder
  if (dif_time_L > 0.001) {
    if (digitalRead(DT_L_PIN) == digitalRead(CLK_L_PIN)) {
      //right += (2 * (2 * PI)) / COUNTS_PER_ROTATION;

      left_AV = (-2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((millis() - TIME) / 1000));
      leftVelocity = left_AV * r;
      rightVelocity = 0;
      if (leftVelocity <= 0) {
        Direction = 1;
        TIME = millis();
      }
    } else if (digitalRead(DT_L_PIN) != digitalRead(CLK_L_PIN)) {
      // right -= (2 * (2 * PI)) / COUNTS_PER_ROTATION;

      left_AV = (2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((millis() - TIME) / 1000));
      leftVelocity = left_AV * r;
      rightVelocity = 0;
      if (leftVelocity >= 0) {
        Direction = 1;
        TIME = millis();
      }
    }

    // TIME = millis();
    //Direction = 1;
  }
}

void CLK_R_ISR() {
  // assign the time
  dif_time_L = millis() - TIME;
  // assign the time

  // find the angular position of the encoder
  if (dif_time_L > 0.001) {
    if (digitalRead(DT_R_PIN) == digitalRead(CLK_R_PIN)) {
      //right += (2 * (2 * PI)) / COUNTS_PER_ROTATION;

      right_AV = (-2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((millis() - TIME) / 1000));
      rightVelocity = right_AV * r;
      leftVelocity = 0;
      if (rightVelocity <= 0) {
        Direction = 1;
        TIME = millis();
      }
    } else if (digitalRead(DT_R_PIN) != digitalRead(CLK_R_PIN)) {
      // right -= (2 * (2 * PI)) / COUNTS_PER_ROTATION;

      right_AV = (2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((millis() - TIME) / 1000));
      rightVelocity = right_AV * r;
      leftVelocity = 0;
      if (rightVelocity >= 0) {
        Direction = 1;
        TIME = millis();
      }
    }
  }
}