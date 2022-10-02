#include <Wire.h>

#define SLAVE_ADDRESS 0x04
int sensorPin = A0;
int state = 0;
int sensorValue = 0 ;
byte data = 0 ;
void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onRequest(sendData);
  pinMode(13, OUTPUT) ;
  Serial.println("Ready!");
}

void loop() {
  sensorValue = analogRead(sensorPin) ;
  data = byte(sensorValue / 4) ;
  Wire.write(12) ;
  digitalWrite(13,HIGH) ;
  delay(sensorValue) ;
  Serial.print(data) ;
}


// callback for sending data
void sendData(){
  Wire.write(data);
}
