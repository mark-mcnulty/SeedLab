String data ;
bool dataRead ;

void setup() {
  Serial.begin(9600) ;
}

void loop() {
  if (dataRead) {
    data = String(data.toInt() + 5) ;
    Serial.print("You sent: ") ;
    Serial.print(data) ;
    dataRead = false ;
  }
}

void serialEvent() {
  if (Serial.available() > 0) {
    data = Serial.readStringUntil("\n") ;
    dataRead = true ;
  }
  Serial.flush() ;
}
