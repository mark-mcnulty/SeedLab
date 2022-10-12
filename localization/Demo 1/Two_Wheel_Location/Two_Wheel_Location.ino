
#include "DualMC33926MotorShield.h"
#include <Wire.h>
#include <Encoder.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#define COUNTS_PER_ROTATION 3200
#define SLAVE_ADDRESS 0x04

//Localization Encoder and Motor decleration
DualMC33926MotorShield md;
//define the  left encoder
int ENCL1 = 3;
int ENCL2 = 4;
Encoder motorLeft (ENCL1, ENCL2;
//define the right enconder
int ENCR1 = 2;
int ENCR2 = 5;
Encoder motorRight (ENCR1, ENCR2);

//Localization position variablles
float thetaLeft = 0.0;
float desiredThetaLeft;
float thetaRight = 0.0;
float desiredThetaRight;


void setup() {
  //Localization setup
   /* 
  * define the motor stuff
  */
  pinMode(enable, OUTPUT);
  pinMode(motor1Dir, OUTPUT);
  pinMode(motor2Dir, OUTPUT);
  pinMode(motor1Volt, OUTPUT);
  pinMode(motor2Volt, OUTPUT);
  pinMode(statusFlag, INPUT);


}
//Need to get in angle convert to radians
//Each wheel rotates desired angle in rads * 2. Each wheel rotates in oposite direction
//Calculate error for each wheel
void loop() {
  //Read time at the start of loop for controls
  time = millis();
  //Getting desired angle and distance from computer vision

  //Convert angle needed to rotate into radians. 
  desiredTheata = (desiredAngle*PI) / 180;
  //Calculate how much each wheel needs to rotate in order to move the robot
  desredThetaLeft = 2*desiredTheta;
  desiredThetaRight = desiredThetaLeft;
  
  //Reading Left Encoder
  thetaLeft= (2*PI* motorLeft.read())/COUNTS_PER_ROTATION;
  Serial.println(thetaLeft);
  //Reading Right Encoder
  thetaRight= (2*PI* motorRight.read())/COUNTS_PER_ROTATION;
  Serial.println(thetaRight);

  //Simulation and Control Calculates Velocity pushed to each wheel. 
  // find the error and the voltage you need to apply 
  errorLeft = desiredThetaLeft - thetaLeft;
  errorRight = desiredThetaRight - thetaRight;

  




}
