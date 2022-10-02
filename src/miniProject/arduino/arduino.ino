
// all the include libraries
#include "DualMC33926MotorShield.h"
#include <Wire.h>


// define the pins
#define CLK_L_PIN 2
#define CLK_R_PIN 3
#define DT_L_PIN 7
#define DT_R_PIN 8
#define D2 4
#define SF 12
#define ANALOG0 'A0'
#define ANALOG1 'A1'

// define constants
#define COUNTS_PER_ROTATION 128
#define SLAVE_ADDRESS 0x04

DualMC33926MotorShield md;

volatile int Direction = 0;

float left = 0.0;
float left_AV = 0.0;
float leftVelocity = 0.0;

float right = 0.0;
float right_AV = 0.0;
float rightVelocity = 0.0;
float TIME = 0.0;
float dif_time_L = 0.0;

const int wait = 138 ;
const int pause = 2000 ;
int state = 0 ;


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
  DualMC33926MotorShield(DT_L_PIN,CLK_L_PIN,ANALOG0,DT_R_PIN,CLK_R_PIN,ANALOG1,D2,SF) ;
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
  if (Direction == 1 && abs(leftVelocity) < 2 && abs(rightVelocity) < 2) {
    //Serial.print(count);
    //Serial.print("\t");
    Serial.print(TIME / 1000000);
    Serial.print("\t");
    Serial.print(left);
    Serial.print("\t");
    Serial.println(right);
    Direction = 0;
    
  }
  delay(100);
}
/*
* This is where the simulation and control functions will go
*/
void CLK_L_ISR() {
  // assign the time
  dif_time_L = micros() - TIME;
  // assign the time

  // find the angular position of the encoder
  if (dif_time_L > 10000) {
    if (digitalRead(DT_L_PIN) == digitalRead(CLK_L_PIN)) {
      right += (2 * (2 * PI)) / COUNTS_PER_ROTATION;

      left_AV = (-2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((micros() - TIME) / 1000000));
      leftVelocity = left_AV * r;
      rightVelocity = 0;
      if (leftVelocity <= 0) {
        Direction = 1;
        TIME = micros();
      }
    } else if (digitalRead(DT_L_PIN) != digitalRead(CLK_L_PIN)) {
      right -= (2 * (2 * PI)) / COUNTS_PER_ROTATION;

      left_AV = (2 * (2 * PI)) / (COUNTS_PER_ROTATION * ((micros() - TIME) / 1000000));
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
// callback for received data
void receiveData(int byteCount){
  state = Wire.read() ;
  int i = 0 ;
  while(Wire.available()) {
    data[i] = Wire.read();
    Serial.print(data[i]) ;
    Serial.print(" ") ;
    i++ ;

    
    
  }
  i-- ;
  Serial.println(" ") ;
}

// callback for sending data
void sendData(){
  if (data[1] != 0) {
    if(data[0] == 0) {
      number = data[1] + 5 ;
      Wire.write(number) ;
    }
    else if (data[0] == 1) {
      number1 = data[1] + 10 ;
      Wire.write(number1) ;
    }
  }
  else {
    Wire.write(data[0] + 5) ;
  }
  number = number + 5 ;
  Wire.write(number);
}