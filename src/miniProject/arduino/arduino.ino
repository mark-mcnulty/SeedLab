
// all the include libraries
#include "DualMC33926MotorShield.h"
#include <Encoder.h>
#include <Wire.h>


// define the pins
#define CLK_L_PIN 2
#define CLK_R_PIN 3
#define DT_L_PIN 7
#define DT_R_PIN 8

// define constants
#define COUNTS_PER_ROTATION 3100
#define SLAVE_ADDRESS 0x04

DualMC33926MotorShield md;
Encoder motorLeft(7,2) ;




//In centimeters Not converted
float r = 7.6;
int count = 0;
int number = 0 ;

float targetTheta = 0.0 ;

void setup() {
  /*
  * simulation and control
  */
  Serial.begin(115200) ;
  Serial.println("Dual MC33926 Motor Shield");
  DualMC33926MotorShield();
  md.init();


  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  //Wire.onReceive(receiveData);
  //Wire.onRequest(sendData);

  Serial.println("Ready!");
  motorLeft.write(0) ;  
}

long positionLeft = -999 ;
float thetaLeft = 0.0;

void loop() {
  long newLeft ;
  newLeft = motorLeft.read();
  
  if (newLeft != positionLeft) {
    Serial.print("Left = ");
    Serial.println(newLeft) ;
    Serial.println(" ") ;
    thetaLeft= (2*PI*newLeft)/3100 ;
    Serial.print(thetaLeft);
    //.print(", Right = ");
    //Serial.print(newRight);
    Serial.println();
    positionLeft = newLeft;
  }
  
  delay(100) ;
  
  if (number == 1) {
    targetTheta = 0;
  }

  if (number == 2) {
    targetTheta = PI/2;
  }

  if (number == 3) {
    targetTheta = PI;
  }

  if (number == 4) {
    targetTheta = (3*PI)/2;
  } 

  if (thetaLeft != targetTheta) {
    if (thetaLeft - targetTheta < 0) {
      md.setM1Speed(100) ;
      delay(100) ;
    }
    if (thetaLeft - targetTheta > 0) {
      md.setM1Speed(-100) ;
      delay(100) ;
    }
  }
  //if (thetaLeft )

  
  //error = desiredTheta - radianLeft;

  //I = I + Ts*error;

  //u = Kp*error + Ki*I;

  //Ts = millis() - Tc;
  //Tc = millis();
  
  // if statement to zero encoder
  // use millis so it works
}
