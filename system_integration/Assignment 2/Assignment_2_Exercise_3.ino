#include <Wire.h>

#define SLAVE_ADDRESS 0x04
int number = 0;
int number1 = 0 ;
int state = 0;
int arrayLen = 0 ;
byte data[32] = {0} ;
byte storeData[32] = {0} ;


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

// callback for received data
void receiveData(int byteCount){
  int i = 0 ;
  while(Wire.available()) {
    data[i] = Wire.read();
    Serial.print(data[i]) ;
    Serial.print(" ") ;
    i++ ;
    arrayLen++ ;
    
  }
  i-- ;
  Serial.println(" ") ;
  int j = 0 ;
  for (int k = i - 1; k != -1; k--) {
    storeData[j] = data[k+1] ;
    j++ ;
  }
}

// callback for sending data
void sendData(){
  Wire.write(storeData, 32) ;
}
