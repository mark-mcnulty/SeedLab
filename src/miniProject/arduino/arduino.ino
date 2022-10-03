
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
#define COUNTS_PER_ROTATION 3100
#define SLAVE_ADDRESS 0x04

DualMC33926MotorShield md;
Encoder motorLeft (5,6) ;


long left = 0;
float desiredThetaLeft;
float left_AV = 0.0;
float leftVelocity = 0.0;
float positionLeft = -999 ;
float thetaLeft = 0.0;
float newLeft = 0;

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
}



void loop() {
  number = 3;
  float Kp, Ki;
  float I = 0.00;
  float e_past = 0.00;
  float Ts = 0.00;
  float Tc = millis();
  float error = 0.00;
  float u;



  thetaLeft= (2*PI* motorLeft.read())/COUNTS_PER_ROTATION;
  Serial.println(thetaLeft);

  
  if (number == 1) {
    desiredThetaLeft = 0;
  }

  if (number == 2) {
    desiredThetaLeft = PI/2;
  }

  if (number == 3) {
    desiredThetaLeft = PI;
  }

  if (number == 4) {
    desiredThetaLeft = (3*PI)/2;
  } 

  //Conditions that drive the motor
  if (thetaLeft != desiredThetaLeft) {
    if (thetaLeft - desiredThetaLeft < 0) {
      md.setM1Speed(100);
      // calculatePosition();
    }
    if (thetaLeft - desiredThetaLeft > 0) {
      md.setM1Speed(-100) ;
      // calculatePosition();
    }
  }

  
  error = desiredThetaLeft - left;

  I = I + Ts*error;

  u = Kp*error + Ki*I;

  Ts = millis() - Tc;
  Tc = millis();
  
  // if statement to zero encoder
  // use millis so it works
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

////Calculates Angular position
// void calculatePosition(){
//   newLeft += motorLeft.read();
//   Serial.println(newLeft);
//   thetaLeft= (2*PI*newLeft)/COUNTS_PER_ROTATION;
//   // Serial.print("Left = ");
//   // Serial.println(thetaLeft) ;
//   // delay(100) ;
// }