
#include "DualMC33926MotorShield.h"
#include <Wire.h>
#include <Encoder.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#define COUNTS_PER_ROTATION 3200
#define SLAVE_ADDRESS 0x04

// define the pins
//HELLO WORLSD
#define ENCL1_LEFT 3
#define ENCL2_LEFT 4
#define ENCR1_RIGHT 2
#define ENCR2_RIGHT 5
Encoder motorRight (ENCR1_RIGHT, ENCR2_RIGHT);
Encoder motorLeft (ENCL1_LEFT, ENCL2_LEFT);

// define the motor pins
int enable = 4;
int motor1Volt = 9;
int motor2Volt = 10;
int motor1Dir = 7;
int motor2Dir = 8;
int statusFlag = 12;

//Localization Encoder and Motor decleration
DualMC33926MotorShield md;


//Localization position variablles
float thetaLeft = 0.0;
float desiredThetaLeft;
float thetaRight = 0.0;
float desiredThetaRight;
float r = 7.6;
float goto_angle = 270;
float goto_position = 0.0;
float goto_theata;
float errorLeft;
float errorRight;
int stepRight;
int stepLeft;
float voltageRight;
float voltageLeft;


// 
float Kp = 4;
float voltage = 0;
float maxVoltage = 7.7;


float shutOffError = 0.05;
float windUpTolerance = PI;
float period = 10;
float start_time;
float t_past;
float Ki = 1.1;
float I = 0.00;
float u;


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
  start_time = millis();

  //Getting desired angle and distance from computer vision

  //Convert angle needed to rotate into radians. 
  // desired angle is in degrees
  goto_theata = (goto_angle*PI) / 180;
  
  //Calculate how much each wheel needs to rotate in order to move the robot
  desiredThetaLeft = 2*goto_theata;
  desiredThetaRight = 2*goto_theata;
  
  //Reading Left Encoder
  thetaLeft= (2*PI* motorLeft.read())/COUNTS_PER_ROTATION;
  // Serial.println(thetaLeft);

  //Reading Right Encoder
  thetaRight= (2*PI* motorRight.read())/COUNTS_PER_ROTATION;
  // Serial.println(thetaRight);

  //Simulation and Control Calculates Velocity pushed to each wheel. 
  // find the error and the voltage you need to apply 
  errorLeft = desiredThetaLeft - thetaLeft;
  errorRight = desiredThetaRight - thetaRight;


  // assign the direction
   if (errorLeft > 0){
    digitalWrite(motor1Dir, LOW);
  } else {
    digitalWrite(motor1Dir, HIGH);
  }

  if (errorRight > 0){
    digitalWrite(motor2Dir, LOW);
  } else {
    digitalWrite(motor2Dir, HIGH);
  }

  // get the error to positive
  errorLeft = abs(errorLeft);
  errorRight = abs(errorRight);


  if (errorLeft > shutOffError){

    //calculate the voltage you need to apply
    if (errorLeft > windUpTolerance){
      voltageLeft = errorLeft * Kp;
    } else {
      I = I + errorLeft*((start_time - t_past)/1000); // convert to millis
      voltageLeft = errorLeft * Kp + Ki*I;
    }

    // error correction shouldn't go over max voltage and anti windup
    if (voltageLeft > maxVoltage) {
      voltageLeft = maxVoltage;
      I = 0;
      // add anti windup properly here LATER
    }

    // assign our steps 
    stepLeft = abs(voltageLeft/maxVoltage) * 255;

    // Conditions that drive the motor
    analogWrite(motor1Volt, stepLeft);
  } else{
    analogWrite(motor1Volt, 0);
  }


//Controls for  the right wheel



  if (errorRight > shutOffError){

    //calculate the voltage you need to apply
    if (errorRight > windUpTolerance){
      voltageRight = errorRight * Kp;
    } else {
      I = I + errorRight*((start_time - t_past)/1000); // convert to millis
      voltageRight = errorRight * Kp + Ki*I;
    }

    // error correction shouldn't go over max voltage and anti windup
    if (voltageRight > maxVoltage) {
      voltageRight = maxVoltage;
      I = 0;
      // add anti windup properly here LATER
    }

    // assign our steps 
    stepRight = abs(voltageRight/maxVoltage) * 255;

    // Conditions that drive the motor
    analogWrite(motor2Volt, stepRight);
  } else{
    analogWrite(motor2Volt, 0);
  }

  //Calculate robot position and velocity using given quations

  t_past = start_time;
  
  // this will add a delay between operations
  while (millis() < start_time + period); // change this to if

}
