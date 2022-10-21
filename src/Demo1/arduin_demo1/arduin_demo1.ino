
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
int motorLVolt = 9;
int motorRVolt = 10;
int motorLDir = 7;
int motorRDir = 8;
int statusFlag = 12;

//Localization Encoder and Motor decleration
DualMC33926MotorShield md;


//Localization position variablles
//All distance and measurements are in centimeters
//All theta variables should be in radians and angle is in degree

//Robot Angle Variables
float goto_theata;
float goto_angle = 270;
float shutOffError = 0.05;

//Left Wheel Variables
float thetaLeft = 0.0;
float desiredThetaLeft;
float errorLeft;
float voltageLeft;
int stepLeft;

//Right Wheel Variable
float thetaRight = 0.0;
float desiredThetaRight;
float errorRight;
float voltageRight;
int stepRight;

//Distance Variable
float distance = 0.0;
float goto_distance = 300;
float errorDistance;
float voltageDistance;
float stepDistance;
float shutOffDistance = 5;

// Controls variables - Should be the first thing messed with
float Kp = 4;
float Ki = 1.1;
float I = 0.00;
float windUpTolerance = PI/2.3;
//Distance Contrrol Variables
float Kp_D = 3.5;
float Ki_D = 1.0;
float I_D = 0.00;

//Constants
float maxVoltage = 7.7;
float period = 10;
float start_time;
float t_past;
float r = 7.6;
float u;


void setup() {
  //Localization setup
   /* 
  * define the motor stuff
  */
  pinMode(enable, OUTPUT);
  pinMode(motorLDir, OUTPUT);
  pinMode(motorRDir, OUTPUT);
  pinMode(motorLVolt, OUTPUT);
  pinMode(motorRVolt, OUTPUT);
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
   if (errorLeft > 0) {
    digitalWrite(motorLDir, LOW);
  } else {
    digitalWrite(motorLDir, HIGH);
  }

  if (errorRight > 0) {
    digitalWrite(motorRDir, LOW);
  } else {
    digitalWrite(motorRDir, HIGH);
  }

  // get the error to positive
  errorLeft = abs(errorLeft);
  errorRight = abs(errorRight);


//Controls for the left wheel 
 
  if (errorLeft > shutOffError) {

    //calculate the voltage you need to apply
    if (errorLeft > windUpTolerance) {
      voltageLeft = errorLeft * Kp;
      I = 0;

    } else {
      I = I + errorLeft*((start_time - t_past)/1000); // convert to millis
      voltageLeft = errorLeft * Kp + Ki*I;
    }

    // error correction shouldn't go over max voltage and anti windup
    if (voltageLeft > maxVoltage) {
      voltageLeft = maxVoltage;
      //I = 0;
      // add anti windup properly here LATER
    }

    // assign our steps 
    stepLeft = abs(voltageLeft/maxVoltage) * 255;

    // Conditions that drive the motor
    analogWrite(motorLVolt, stepLeft);
  } else {
    analogWrite(motorLVolt, 0);
  }


//Controls for  the right wheel

  if (errorRight > shutOffError) {

    //calculate the voltage you need to apply
    if (errorRight > windUpTolerance) {
      voltageRight = errorRight * Kp;
      I = 0;
    } else {
      I = I + errorRight*((start_time - t_past)/1000); // convert to millis
      voltageRight = errorRight * Kp + Ki*I;
    }

    // error correction shouldn't go over max voltage and anti windup
    if (voltageRight > maxVoltage) {
      voltageRight = maxVoltage;
      //I = 0;
      // add anti windup properly here LATER
    }

    // assign our steps 
    stepRight = abs(voltageRight/maxVoltage) * 255;

    // Conditions that drive the motor
    analogWrite(motorRVolt, stepRight);
  } else {
    analogWrite(motorRVolt, 0);
  }

  //Calculate robot position and velocity using given quations

  t_past = start_time;
  
  // this will add a delay between operations
  while (millis() < start_time + period); // change this to if


  /*
  
  // This part will make the robot travel forward
  
  //First get the distance and ccalculate the error in distance
  errorDistance = goto_distance - distance;

  //Set direction of the two motors
  if (errorDistance > 0 ) {
    digitalWrite(motorLDir, LOW);
    digitalWrite(motorRDir, HIGH);

  } else {
    digitalWrite(motorLDir, HIGH);
    digitalWrite(motorRDir, LOW);
  }

  errorDistance = abs(errorDistance);

  //Control the robot to move
  if (errorDistance >= shutOffDistance) {
   //Reset the position of each wheel to 0
    thetaLeft = 0;
    thetaRight = 0;
  
    //Controls calculates the Kp Ki and voltages 
  if (errorDistance > shutOffDistance) {

    //calculate the voltage you need to apply
    if (errorDistance > windUpTolerance) {
      voltageDistance = errorDistance * Kp;
    } else {
      I_D = I_D + errorDistance*((start_time - t_past)/1000); // convert to millis
      voltageDistance = errorDistance * Kp_D + Ki_D*I_D;
    }

    // error correction shouldn't go over max voltage and anti windup
    if (voltageDistance > maxVoltage) {
      voltageDistance = maxVoltage;
      I_D = 0;
      // add anti windup properly here LATER
    }

    // assign our steps 
    stepDistance = abs(voltageDistance/maxVoltage) * 255;

    // Conditions that drive the motor
    analogWrite(motorLVolt, stepDistance);
    analogWrite(motorRVolt, stepDistance);

    //Calculate distance the robot has traveled
    distance = (r/2)*(thetaLeft + thetaRight);
    
  } else{
    analogWrite(motorLVolt, 0);
    analogWrite(motorRVolt, 0);
  }
  

  }

  */

}
