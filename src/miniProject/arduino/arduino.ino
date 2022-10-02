
// all the include libraries
#include "DualMC33926MotorShield.h"
#include <Wire.h>


// define the pins
#define CLK_L_PIN 2
#define CLK_R_PIN 3
#define DT_L_PIN 7
#define DT_R_PIN 8

// define constants
#define COUNTS_PER_ROTATION 3200
#define SLAVE_ADDRESS 0x04

DualMC33926MotorShield md;

volatile int Direction = 0;

int period = 10;
float left = 0.0;
float desiredTheta;
float left_AV = 0.0;
float leftVelocity = 0.0;

float right = 0.0;
float desiredRight = 0.0;
float right_AV = 0.0;
float rightVelocity = 0.0;

float TIME = 0.0;
float dif_time_L = 0.0;

const int wait = 138 ;
const int pause = 2000 ;

// this is for system integration
int number = 0;
int data[32] = {0} ;


//In centimeters Not converted
float r = 7.6;
int count = 0;

void setup() {
  /*
  * simulation and control
  */
  Serial.begin(115200) ;
  attachInterrupt(digitalPinToInterrupt(CLK_L_PIN), CLK_L_ISR, CHANGE);  // change rising or falling
  attachInterrupt(digitalPinToInterrupt(CLK_R_PIN), CLK_R_ISR, CHANGE);  // pin, function, flag to look for
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
}


void loop() {
  float Kp, Ki;
  float I = 0.00;
  float e_past = 0.00;
  float Ts = 0.00;
  float Tc = millis();
  float error = 0.00;
  float u;
  
  if (number == 1) {
    desiredTheta = 0;
  }

  if (number == 2) {
    desiredTheta = PI/2;
  }

  if (number == 3) {
    desiredTheta = PI;
  }

  if (number == 4) {
    desiredTheta = (3*PI)/2;
  } 

  
  error = desiredTheta - left;

  I = I + Ts*error;

  u = Kp*error + Ki*I;

  Ts = millis() - Tc;
  Tc = millis();
  
  // if statement to zero encoder
  // use millis so it works
  while (millis() < Tc + period);
}


void CLK_L_ISR() {
  // assign the time
  dif_time_L = micros() - TIME;
  // assign the time

  // find the angular position of the encoder
  if (dif_time_L > 10000) {
    if (digitalRead(DT_L_PIN) == digitalRead(CLK_L_PIN)) {
      left += (2 * (2 * PI)) / COUNTS_PER_ROTATION; // In radians

      left_AV = (2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((micros() - TIME) / 1000000));
      leftVelocity = left_AV * r;
      rightVelocity = 0;
      if (leftVelocity <= 0) {
        Direction = 1;
        TIME = micros();
      }
    } else if (digitalRead(DT_L_PIN) != digitalRead(CLK_L_PIN)) {
      left -= (2 * (2 * PI)) / COUNTS_PER_ROTATION;

      left_AV = (-2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((micros() - TIME) / 1000000));
      leftVelocity = left_AV * r;
      rightVelocity = 0;
      if (leftVelocity >= 0) {
        Direction = 1;
        TIME = micros();
      }
    }

    // TIME = millis();
    //Direction = 1;
  }
}

void CLK_R_ISR() {
  // assign the time
  dif_time_L = micros() - TIME;
  // assign the time

  // find the angular position of the encoder
  if (dif_time_L > 10000) {
    if (digitalRead(DT_R_PIN) == digitalRead(CLK_R_PIN)) {
      right += (2 * (2 * PI)) / COUNTS_PER_ROTATION;

      right_AV = (-2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((micros() - TIME) / 1000000));
      rightVelocity = right_AV * r;
      leftVelocity = 0;
      if (rightVelocity <= 0) {
        Direction = 1;
        TIME = micros();
      }
    } else if (digitalRead(DT_R_PIN) != digitalRead(CLK_R_PIN)) {
      right -= (2 * (2 * PI)) / COUNTS_PER_ROTATION;

      right_AV = (2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((micros() - TIME) / 1000000));
      rightVelocity = right_AV * r;
      leftVelocity = 0;
      if (rightVelocity >= 0) {
        Direction = 1;
        TIME = micros();
      }
    }
  }
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