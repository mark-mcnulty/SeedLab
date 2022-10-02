#include <Wire.h>

#define SLAVE_ADDRESS 0x04
int number = 0;
int number1 = 0 ;
int state = 0;
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

// callback for received data
void receiveData(int byteCount){
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
