
// #include "DualMC33926MotorShield.h"
#include <Wire.h>
#include <Encoder.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#define COUNTS_PER_ROTATION 3200
#define SLAVE_ADDRESS 0x04

// define the pins
//HELLO WORLSD
// DualMC33926MotorShield md;
#define ENCL1_LEFT 2
#define ENCL2_LEFT 5
#define ENCR1_RIGHT 3
#define ENCR2_RIGHT 6
Encoder motorRight (ENCR1_RIGHT, ENCR2_RIGHT);
Encoder motorLeft (ENCL1_LEFT, ENCL2_LEFT);

// define the motor pins
int enable = 4;
int motorLDir = 7;
int motorRDir = 8;
int motorLVolt = 9;
int motorRVolt = 10;
int statusFlag = 12;

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
float errorDistance;
float voltageDistance;
float stepDistance;
float shutOffDistance = 5;

// Controls variables
float shutOffError = 0.05;
float Kp = 4;
float Ki = 1.1;
float I = 0.00;
float windUpTolerance = PI/2.3;
float Kp_slave = 3.5;
float Ki_slave = 1.0;
float I_slave = 0.00;



void setup() {
  Serial.begin(115200) ;
  // set the pins
  pinMode(enable, OUTPUT);
  pinMode(motorLDir, OUTPUT);
  pinMode(motorRDir, OUTPUT);
  pinMode(motorLVolt, OUTPUT);
  pinMode(motorRVolt, OUTPUT);
  pinMode(statusFlag, INPUT);

  digitalWrite(motorLDir, HIGH);
  digitalWrite(motorRDir, HIGH);
  digitalWrite(enable, HIGH);
}

// variables
float errorSlave;
float masterVoltage;

float goto_theata;
float goto_angle = 270;

float goto_distance = 300;
float distance = 0.0;

float maxVoltage = 7.7;
float period = 10;
float time_start;
float time_past;
float r = 7.6;
float u;

float count; 
float voltage = 5;

// void loop
void loop() {
  // test direction
  count = abs(voltage/7.5) * 255;


  //write to the motors
  analogWrite(motorRVolt, 75);
  analogWrite(motorLVolt, 75);

  // display the encoder data 
  Serial.print(motorLeft.read());
  Serial.print(" ");
  Serial.print(motorRight.read());
  Serial.print("\n");

}


/*
  calc_degrees
  PARAMETERS: float
  RETURN: float
  FUNCTIONALITY: This function will return the degrees of a radian
*/
float calc_degrees(float rad){
  return (rad * 180) / PI;
}


/*

  calc_radians
  PARAMETERS: float
  RETURN: float
  FUNCTIONALITY: This function will calculate the degrees and return it

*/
float calc_radians(float deg) {
  return (deg * PI) / 180; 
}

/*
  drive_right()
  PARAMETERS: None
  RETURN: None
  FUNCTIONALITY: This will set the controls to drive right
*/
void drive_right(){
  digitalWrite(motorLDir, LOW);
  digitalWrite(motorRDir, LOW);
}

/*
  drive_left()
  PARAMETERS: None
  RETURN: None
  FUNCTIONALITY: This will set the controls to drive left
*/
void drive_left(){
  digitalWrite(motorLDir, LOW);
  digitalWrite(motorRDir, LOW);
}

/*
  drive_forward()
  PARAMETERS: None
  RETURN: None
  FUNCTIONALITY: This will set the controls to drive forward
*/
void drive_forward(){
  digitalWrite(motorLDir, LOW);
  digitalWrite(motorRDir, LOW);
}

/*
  drive_right()
  PARAMETERS: None
  RETURN: None
  FUNCTIONALITY: This will set the controls to drive backward
*/
void drive_backward(){
  digitalWrite(motorLDir, LOW);
  digitalWrite(motorRDir, LOW);
}

/* 
  drive_master()
  PARAMETERS: 
    float error -> This is the error (radians) of the current position to desired position. 
    float time_start -> This is the time that the loop started at. For integrator.
    float time_past -> This is the time at the end of the loop. For integrator.
  RETURN: 
  FUNCTIONALITY: this will set the voltage on the right wheel based on the error
*/
void drive_master(float error, float time_start, float time_past){
  // define variables
  float voltage = 0;

  // assign the direction based on error sign
  if (error > 0) {
    digitalWrite(motorRDir, LOW);
  } else {
    digitalWrite(motorRDir, HIGH);
  }

  // get the abs value of the error now that direction has been assigned
  error = abs(error);

  // only drive the motor if the error is greater then the shutofferror
  if(error < shutOffError){
    // shut the control off
    analogWrite(motorRVolt, 0);
  } else {
    
    // implement the control to set the voltage of the motor
    // implement windUpTolerance
    if (error > windUpTolerance){
      // dont use the integrator
      voltage = error * Kp;

    } else {
      // use integrator
      I = I + error * ((time_start - time_past)/1000);
      voltage = (error * Kp) + (Ki * I);
    }

    // make sure the voltage isn't more then the max voltage
    if (voltage > maxVoltage){
      voltage = maxVoltage;
    }

    // assign the voltage value to the motor
    analogWrite(motorRVolt, (abs(voltage/maxVoltage) * 255));
  }

  return voltage;
}

/* 
  drive_slave()
  PARAMETERS:
  RETURN:
  FUNCTIONALITY: this will set the voltage on the right wheel based on the error
*/
void drive_slave(float error, float masterVoltage, float time_start, float time_past){
  // define variables
  float voltage;

  // assign the direction based on error sign
  if (error > 0) {
    digitalWrite(motorLDir, LOW);
  } else {
    digitalWrite(motorLDir, HIGH);
  }

  // get the abs value of the error now that direction has been assigned
  error = abs(error);

  // only drive the motor if the error is greater then the shutofferror
  if(error < shutOffError){
    // shut the control off
    analogWrite(motorRVolt, 0);
  } else {
    
    // implement the control to set the voltage of the motor
    // implement windUpTolerance
    if (error > windUpTolerance){
      // dont use the integrator
      voltage = error * Kp;

    } else {
      // use integrator
      I = I + error * ((time_start - time_past)/1000);
      voltage = (error * Kp) + (Ki * I);
    }

    // make sure the voltage isn't more then the max voltage
    if (voltage > maxVoltage){
      voltage = maxVoltage;
    }

    // assign the voltage value to the motor
    analogWrite(motorRVolt, (abs(voltage/maxVoltage) * 255));
  }
}



