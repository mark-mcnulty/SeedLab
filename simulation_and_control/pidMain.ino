
// all the include libraries
#include "DualMC33926MotorShield.h"
#include <Wire.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

// define the pins
// #define CLK_L_PIN 2
// #define CLK_R_PIN 3
// #define DT_L_PIN 7
// #define DT_R_PIN 8

// define constants
#define COUNTS_PER_ROTATION 3200
#define SLAVE_ADDRESS 0x04

//define the encoder
int ENCA = 3;
int ENCB = 2;
DualMC33926MotorShield md;
Encoder motorLeft (ENCA, ENCB) ;//5,6 originally

// define the motor pins
int enable = 4;
int motor1Volt = 9;
int motor2Volt = 10;
int motor1Dir = 7;
int motor2Dir = 8;
int statusFlag = 12;



// localization stuff
// DONT TOUCH
float thetaLeft = 0.0;
float desiredThetaLeft;
float thetaRight = 0.0;
float TIME = 0.0;
float dif_time_L = 0.0;
const int wait = 138 ;
const int pause = 2000 ;
float r = 7.6;
int count = 0;

// this is for system integration
// DONT TOUCH
int number = 4;
int data[32] = {0} ;


// this is simulation and control 
// DONT TOUCH IF YOU ARENT THAT SUBSYSTEM!!!!
float Kp = 4;
float voltage = 0;
float maxVoltage = 7.7;
float error = 0.00;
int step;

float shutOffError = 0.05;
float windUpTolerance = PI;
float period = 10;
float time;
float t_past;
float Ki = 1.1;
float I = 0.00;
float u;


void setup() {
  /* 
  * define the motor stuff
  */
  pinMode(enable, OUTPUT);
  pinMode(motor1Dir, OUTPUT);
  pinMode(motor2Dir, OUTPUT);
  pinMode(motor1Volt, OUTPUT);
  pinMode(motor2Volt, OUTPUT);
  pinMode(statusFlag, INPUT);


  /*
  * simulation and control
  */
  Serial.begin(115200) ;
  //attachInterrupt(digitalPinToInterrupt(CLK_L_PIN), CLK_L_ISR, CHANGE);  // change rising or falling
  //attachInterrupt(digitalPinToInterrupt(CLK_R_PIN), CLK_R_ISR, CHANGE);  // pin, function, flag to look for
  Serial.println("Dual MC33926 Motor Shield");
  DualMC33926MotorShield();
  md.init();

  /*
  *
  * Here is where the void setup for the system integration stuff is
  * 
  */
 // the output pin to the raspberry pi
  pinMode(13, OUTPUT);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");

  digitalWrite(enable, HIGH);
  // LOW == counter clock wise... negative radians
  // HIGH == clock wise... positive radians
  // digitalWrite(motor1Dir, LOW);
  // analogWrite(motor1Volt, 255);
}



void loop() {
  // assign values
  time = millis();
  thetaLeft= (2*PI* motorLeft.read())/COUNTS_PER_ROTATION;
  //Serial.println(thetaLeft);

  // find the position it needs to go to 
  if (number == 1) {
    desiredThetaLeft = 0;
  }else if (number == 2) {
    desiredThetaLeft = PI/2;
  }else if (number == 3) {
    desiredThetaLeft = PI;
  }else if (number == 4) {
    desiredThetaLeft = (3*PI)/2;
  } 

  
  // // SIMULATION AND CONTROL
  // find the error and the voltage you need to apply 
  error = desiredThetaLeft - thetaLeft;

  // find the direction the wheel needs to spin
  if (error > 0){
    digitalWrite(motor1Dir, LOW);
  } else {
    digitalWrite(motor1Dir, HIGH);
  }
  // get the error to positive
  error = abs(error);

  if (error > shutOffError){

    //calculate the voltage you need to apply
    if (error > windUpTolerance){
      // Serial.println("error");
      // Serial.println(error);
      voltage = error * Kp;
    } else {
      I = I + error*((time - t_past)/1000); // convert to millis

      // Serial.println("Integral");
      //Serial.println(I);

      voltage = error * Kp + Ki*I;
    }

    // error correction shouldn't go over max voltage and anti windup
    if (voltage > maxVoltage) {
      voltage = maxVoltage;
      I = 0;
      // add anti windup properly here LATER
    }

    // assign our steps 
    step = abs(voltage/maxVoltage) * 255;

    // Conditions that drive the motor
    analogWrite(motor1Volt, step);
  } else{
    analogWrite(motor1Volt, 0);
  }

    t_past = time;
  
  // this will add a delay between operations
  while (millis() < time + period); // change this to if

}

/*
 here is where all the system integration functions will go

*/
// callback for received data from the raspberry pi
void receiveData(int byteCount){
  int i = 0 ;
  while(Wire.available()) {
    data[i] = Wire.read();
    if (i == 1){
      number = data[1];
    }
    i++ ;  
  }
  Serial.println(" ") ;
}


// callback for sending data to the raspberry pi
void sendData(){
    Wire.write(number);
}
