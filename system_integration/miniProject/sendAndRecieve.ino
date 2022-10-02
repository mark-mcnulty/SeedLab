#include <Wire.h>

#define SLAVE_ADDRESS 0x04
int number = 0;
int data[32] = {0} ;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}

void loop() {
  delay(100);
}

// callback for received data from the raspberry pi
void receiveData(int byteCount){
  int i = 0 ;
  while(Wire.available()) {
    data[i] = Wire.read();
    Serial.print(data[i]) ;
    Serial.print(" ") ;
    i++ ;  
  }
  Serial.println(" ") ;
}


// callback for sending data to the raspberry pi
void sendData(){
    Wire.write(number);
}
