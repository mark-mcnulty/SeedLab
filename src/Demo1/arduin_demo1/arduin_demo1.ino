
#include "DualMC33926MotorShield.h"
#include <Wire.h>
#include <Encoder.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#define COUNTS_PER_ROTATION 3200
#define SLAVE_ADDRESS 0x04

// define the motor pins
int enable = 4;
int motor1Volt = 9;
int motor2Volt = 10;
int motor1Dir = 7;
int motor2Dir = 8;
int statusFlag = 12;

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
float r = 7.6;


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

  //Control the left wheel

   if (errorLeft > 0){
    digitalWrite(motor1Dir, LOW);
  } else {
    digitalWrite(motor1Dir, HIGH);
  }

  // get the error to positive
  errorLeft = abs(errorLeft);

  if (errorLeft > shutOffErrorLeft){

    //calculate the voltage you need to apply
    if (errorLeft > windUpToleranceLeft){
      voltageLeft = errorLeft * KpLeft;
    } else {
      ILeft = ILeft + errorLeft*((time - t_past)/1000); // convert to millis
      voltageLeft = errorLeft * KpLeft + KiLeft*ILeft;
    }

    // error correction shouldn't go over max voltage and anti windup
    if (voltageLeft > maxVoltage) {
      voltageLeft = maxVoltage;
      ILeft = 0;
      // add anti windup properly here LATER
    }

    // assign our steps 
    stepLeft = abs(voltageLeft/maxVoltage) * 255;

    // Conditions that drive the motor
    analogWrite(motor1Volt, step);
  } else{
    analogWrite(motor1Volt, 0);
  }


//Controls for  the right wheel
   if (errorRight > 0){
    digitalWrite(motor2Dir, LOW);
  } else {
    digitalWrite(motor2Dir, HIGH);
  }

  // get the error to positive
  errorRight = abs(errorRight);

  if (errorRight > shutOffErrorRight){

    //calculate the voltage you need to apply
    if (errorRight > windUpToleranceRight){
      voltageRight = errorRight * KpRight;
    } else {
      IRight = IRight + errorRight*((time - t_past)/1000); // convert to millis
      voltageRight = errorRight * KpRight + KiRight*IRight;
    }

    // error correction shouldn't go over max voltage and anti windup
    if (voltageRight > maxVoltage) {
      voltageRight = maxVoltage;
      IRight = 0;
      // add anti windup properly here LATER
    }

    // assign our steps 
    stepRight = abs(voltageRight/maxVoltage) * 255;

    // Conditions that drive the motor
    analogWrite(motor2Volt, step);
  } else{
    analogWrite(motor2Volt, 0);
  }

//Calculate robot position and velocity using given quations

    t_past = time;
  
  // this will add a delay between operations
  while (millis() < time + period); // change this to if




}
