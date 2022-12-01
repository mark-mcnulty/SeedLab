#include <Wire.h>
#include <Encoder.h>
// #include <String.h>

#define SLAVE_ADDRESS 0x04

float r = 7.5;      // wheel radius in cm
float markerAngle = 0;
float markerAngleRad = 0;
float markerDistance = 0;
float markerDistanceTheta = 0;
int driveCorrect = 0; // cm

volatile bool markerFound = false;

// for i2c communication
char temp[32];
String command;
String swag ;
String distanceTemp ;
String angleTemp ;
float distance ;
float angle ;
int index ;
int len ;
size_t sz ;


void setup(void) {
    Serial.begin(9600); 

    // setup communication
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);

    // define callbacks for i2c communication
    Wire.onReceive(receiveEvent);
}


void loop(void) {


}

void receiveEvent(int howMany) {

  for (int i = 0; i < howMany; i++) {
    temp[i] = Wire.read();
    temp[i + 1] = '\0'; //add null after ea. char
  }


  //RPi first byte is cmd byte so shift everything to the left 1 pos so temp contains our string
  for (int i = 0; i < howMany; ++i) {
    temp[i] = temp[i + 1];
  }
    // getting the right data here

  swag = temp ;
  index = swag.indexOf(' ') ;
  len = swag.length() ;
  distanceTemp = swag.substring(0,index) ;
  angleTemp = swag.substring(index + 1, len) ;

  markerDistanceTheta = (distanceTemp.toFloat()) / r ;
  markerAngleRad = (angleTemp.toFloat() * PI) / 180 ;
  markerFound = true;

}