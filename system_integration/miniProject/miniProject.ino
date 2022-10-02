#include "DualMC33926MotorShield.h"
 
DualMC33926MotorShield md;

//const int inPin = 12 ;
const int wait = 138 ;
const int pause = 2000 ;
 
void setup()
{
  Serial.begin(115200);
  Serial.println("Dual MC33926 Motor Shield");
  DualMC33926MotorShield() ;
  md.init();
  //pinMode(inPin, INPUT) ;
}
 
void loop()
{
  for (int i = 0; i < 4; i++) {
    md.setM1Speed(400) ;
    delay(wait) ;
    md.setM1Speed(0) ;
    delay(pause) ;
  }
 
  
 
  //value = (analogRead(inPin)/4) ;
 
}
