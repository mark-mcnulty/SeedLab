
#include "DualMC33926MotorShield.h"
#include <Wire.h>
#include <Encoder.h>
#include <String.h>


#define ENCODER_OPTIMIZE_INTERRUPTS
#define COUNTS_PER_ROTATION 3200
#define SLAVE_ADDRESS 0x04

// define the pins
DualMC33926MotorShield md;
#define ENCL1_LEFT 2
#define ENCL2_LEFT 5
#define ENCR1_RIGHT 3
#define ENCR2_RIGHT 6
Encoder motorRight (ENCR1_RIGHT, ENCR2_RIGHT);
Encoder motorLeft (ENCL1_LEFT, ENCL2_LEFT);

// define the motor pins
int enable = 4;
int motorLVolt = 9;
int motorRVolt = 10;
int motorLDir = 7;
int motorRDir = 8;
int statusFlag = 12;

// robot viarbales
float r = 7.5;
float AXIAL = 36.5;            //36.5
float r_a = AXIAL / 2;
float u;

//Distance Variable
float shutOffDistance = 5;

// Controls variables
float shutOffError = 0.03;
float Kp = 2.7; // best 2.9
float Ki = 3.0; // best 3.0
float I = 0.00;

float Kp_slave = 3.3; //best 2.7  2.5 
float Ki_slave = 1.1; // formerly 1.1
float I_slave = 0.00;

float windUpTolerance = PI/2;         //PI/2.3
int MAX_PWM = 175;

float maxVoltage = 7.7;
float maxBattery = 7.7;
float maxVoltageMaster = 6;
float maxVoltageSlave = 7.7;

float period = 10;
float time_start;
float time_past;


// state variables
typedef enum {
    start,
    is_marker_found,
    turn_to_marker,
    drive_to_marker,
    turn_to_find,
    stop
    } states;
states state = start;

// flags for states
volatile bool markerFound = true;
bool turnDone = false;
bool driveDone = false;
bool turnDriveDone = false;
float driveDoneTime = 0;
float turnDoneTime = 0;
float deltaDone = 1500;  // time in ms
float currentTime = 0;

// data from pi about marker
float markerAngle = 0;
float markerAngleRad = PI/2;
float noMarkerAngle = PI/6;       // rads
float markerDistance = 100;
float markerDistanceTheta = markerDistance / r;
int driveCorrect = 3; // cm

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



void setup() {
  // Serial.begin(115200) ;
  Serial.begin(9600);

  // set the pins
  pinMode(enable, OUTPUT);
  pinMode(motorLDir, OUTPUT);
  pinMode(motorRDir, OUTPUT);
  pinMode(motorLVolt, OUTPUT);
  pinMode(motorRVolt, OUTPUT);
  pinMode(statusFlag, INPUT);


  digitalWrite(enable, HIGH);
  analogWrite(motorLVolt, 0);
  analogWrite(motorRVolt, 0);

  // setup communication
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

   // define callbacks for i2c communication
  Wire.onReceive(receiveEvent);
//   // Wire.onRequest(requestEvent);
}



void loop() {
    // define the time_start
    time_start = millis();


    // control unit
    switch (state) {
        case start:
            state = is_marker_found;
            Serial.println("Start");
            break;
        case is_marker_found:
            Serial.println("is_marker_found");
            motorRight.write(0);
            motorLeft.write(0);
            if (markerFound) {
                state = turn_to_marker;
            } else {
                state = turn_to_find;
            }
            break;
        case turn_to_find:
            // windUpTolerance = PI/6.5;
            Serial.println("turn_to_find");
            if (turnDone) {
                state = is_marker_found;
                turnDone = false;

                // reset motor position
                motorRight.write(0);
                motorLeft.write(0);
                I_slave = 0;
                I = 0;
            } 
            break;
        case turn_to_marker:
            // Controls variables
            shutOffError = 0.01;
            Kp = 2.7; // best 2.9
            Ki = 3.0; // best 3.0
            // I = 0.00;

            Kp_slave = 2.5; //best 2.7  2.5 
            Ki_slave = 2.0; // formerly 1.1
            // I_slave = 0.00;

            windUpTolerance = PI;         //PI/2
            MAX_PWM = 175;
            // float windUpTolerance = PI/2;
            Serial.println("turn_to_marker");
            if (turnDone) {
                Serial.println("turn_to_marker_done");
                state = drive_to_marker;
                motorRight.write(0);
                motorLeft.write(0);
                I_slave = 0;
                I = 0;
            } 
            break;
        case drive_to_marker:

            // Controls variables
            shutOffError = 0.01;
            Kp = 2.3; // best 2.9
            Ki = 3.0; // best 3.0
            // I = 0.00;

            Kp_slave = 3.5; //best 2.7  2.5 
            Ki_slave =  1.0; // formerly 1.1
            // I_slave = 0.00;

            windUpTolerance = PI/2;         //PI/2.3
            MAX_PWM = 175;
            Serial.println("drive_to_marker");
            if (driveDone) {
                Serial.println("drive done");
                state = is_marker_found;
                
                // reset the marker found flag
                turnDone = false;
                driveDone = false;
                markerFound = false;
                turnDriveDone = true;

                motorRight.write(0);
                motorLeft.write(0);
                I_slave = 0;
                I = 0;
            }
            break;
        case stop:
            break;
        default:
            break;
    }

    // datapath
    switch (state) {
        // start
        case start:
            break;

        // if the marker is found
        case is_marker_found:
            break;

        // turn to find the marker
        case turn_to_find:
            turn(noMarkerAngle, time_start, time_past);
            break;

        // turn to the marker if the marker is found
        case turn_to_marker:
            // Serial.println(markerAngleRad);
            turn(markerAngleRad, time_start, time_past);
            break;

        // drive to the marker if the marker is found
        case drive_to_marker:
            // Serial.println(markerDistanceTheta);            
            drive(markerDistanceTheta, time_start, time_past);
            break;

        // stop
        case stop:
            break;
        default:
            Serial.println("defalut");
            break;
    }


    //Calculate robot position and velocity using given quations
  time_past = time_start;
  
  // this will add a delay between operations
  while (millis() < time_start + period); // change this to if
}
/* 
DATA FROM PI
*/
void receiveEvent(int howMany) {

  for (int i = 0; i < howMany; i++) {
    temp[i] = Wire.read();
    temp[i + 1] = '\0'; //add null after ea. char
  }

  //RPi first byte is cmd byte so shift everything to the left 1 pos so temp contains our string
  for (int i = 0; i < howMany; ++i) {
    temp[i] = temp[i + 1];
  }
  swag = temp ;
  index = swag.indexOf(' ') ;
  len = swag.length() ;
  distanceTemp = swag.substring(0,index) ;
  angleTemp = swag.substring(index, len - 1) ;
  markerDistanceTheta = (distanceTemp.toFloat() - driveCorrect) / r ;
  markerAngleRad = (angleTemp.toFloat() * PI) / 180 ;
  markerFound = true;


  Serial.print("distance: ");
  Serial.print(markerDistanceTheta);
  Serial.println();
  Serial.print("angle: ");
  Serial.print(markerAngleRad);
  Serial.println();
}

/* 
SENDING TO PI
*/
// void requestEvent() {
//   Serial.println("requested");
//   if(turnDriveDone == true){
//     Wire.write("1");
//     turnDriveDone = false;
//   }
//   else{
//     Wire.write("0");
//   }
// }


/* 
TURN STATE
*/
void turn(float desiredThetaTurn, float time_start, float time_past) {
  // set varaibles
  float masterVoltage = 0;
  float thetaLeft= (2*PI* motorLeft.read()) / COUNTS_PER_ROTATION;
  float thetaRight= (2*PI* motorRight.read()) / COUNTS_PER_ROTATION;

  // turn
  masterVoltage = turn_master(thetaLeft, thetaRight, desiredThetaTurn, time_start, time_past);
  turn_slave(thetaLeft, thetaRight, desiredThetaTurn, masterVoltage, time_start, time_past);
}

/*
DRIVE STATE
*/
void drive(float desiredTheta, float time_start, float time_past) {
  // Serial.println("drive");
  // set variables
  float masterVoltage = 0;
  float thetaLeft= (2*PI* motorLeft.read()) / COUNTS_PER_ROTATION;
  float thetaRight= (2*PI* motorRight.read()) / COUNTS_PER_ROTATION;

  // drive
  masterVoltage = drive_master(thetaLeft, thetaRight, desiredTheta, time_start, time_past);
  drive_slave(thetaLeft, thetaRight, desiredTheta, masterVoltage, time_start, time_past);

}

/* 
  drive_master()
  PARAMETERS: 
    float error -> This is the error (radians) of the current position to desired position. 
    float time_start -> This is the time that the loop started at. For integrator.
    float time_past -> This is the time at the end of the loop. For integrator.
  RETURN: 
  FUNCTIONALITY: this will set the voltage on the Left wheel based on the error
*/
float drive_master(float masterTheta, float SlaveTheta, float desiredTheta, float time_start, float time_past){
  // define variables
  float voltage = 0;
  float error;

  // find error 
  error = desiredTheta - masterTheta;

  // assign the direction based on error sign
  if (error > 0) {
    digitalWrite(motorLDir, HIGH);
  } else {
    digitalWrite(motorLDir, LOW);
  }

  // get the abs value of the error now that direction has been assigned
  error = abs(error);

  // only drive the motor if the error is greater then the shutofferror
  if(error < shutOffError){
    // assign current time
    currentTime = millis();
    // Serial.println("error");

    // check if the time is greater then the delta time
      if (currentTime > driveDoneTime + deltaDone) {
      // set the drive done flag
      Serial.println("state finish");
      driveDone = true;
    }

    // shut the control off
    analogWrite(motorLVolt, 0);

  } else {
    // set flag
    driveDone = false;

    // implement the control to set the voltage of the motor
    // implement windUpTolerance
    if (error > windUpTolerance){
      // dont use the integrator
      I = 0;
      voltage = error * Kp;

    } else {
      // use integrator
      I = I + error * ((time_start - time_past)/1000);
      voltage = (error * Kp) + (Ki * I);
    }

    // make sure the voltage isn't more then the max voltage
    if (voltage > maxVoltageMaster){
      voltage = maxVoltageMaster;
    }
    // assign the voltage value to the motor
    analogWrite(motorLVolt, round(abs(voltage/maxBattery) * MAX_PWM));
    Serial.print("Master: ");
    Serial.print(voltage);
    Serial.print("\t");
    driveDoneTime = millis();
  }


  // return the voltage for the slave 
  return voltage;
}

/* 
  drive_slave()
  PARAMETERS:
  RETURN:
  FUNCTIONALITY: this will set the voltage on the right wheel based on the error
*/
void drive_slave(float masterTheta, float slaveTheta, float desiredTheta, float masterVoltage, float time_start, float time_past){
  // define variables
  float voltage = 0;
  float error;

  // assign error
  error = masterTheta - slaveTheta;

  // assign the direction based on error sign
  if (error > 0) {
    digitalWrite(motorRDir, HIGH);
  } else {
    digitalWrite(motorRDir, LOW);
  }

  // get the abs value of the error now that direction has been assigned
  error = abs(error);

  // only drive the motor if the error is greater then the shutofferror
  if(error < shutOffError){
    
    // shut the control off
    analogWrite(motorRVolt, 0);

  } else {

    // implement the control to set the voltage of the motor
    // implement windUpTolerance
    if (error > windUpTolerance){
      // dont use the integrator
      I_slave = 0;
      voltage = error * Kp_slave;
    } else {
      // use integrator
      I_slave = I_slave + error * ((time_start - time_past)/1000);
      voltage = (error * Kp_slave) + (Ki_slave * I_slave);
    }

    // add in the masters voltage
    voltage = voltage + masterVoltage;

    // make sure the voltage isn't more then the max voltage
    if (voltage > maxVoltageSlave){
      voltage = maxVoltageSlave;
    }

    // assign the voltage value to the motor
    analogWrite(motorRVolt, round(abs(voltage/maxBattery) * MAX_PWM));
    Serial.print("follower: ");
    Serial.print(voltage);
    Serial.println();
  }
}


/*
  float turn_master(float masterTheta, float slaveTheta, float desiredTheta, float time_start, float time_past)
  PARAMTERS:
  RETURN: float masterVolt
  FUNCTIONALITY:
*/
float turn_master(float masterTheta, float slaveTheta, float desiredTheta, float time_start, float time_past) {
  // define variables needed
  float voltage = 0;
  float theta = (r_a * desiredTheta) / r; ;
  float error;

  // need to multiply the desired theta by 2 for turning
  // desiredTheta = ROTATIONAL_GAIN_VALUE * desiredTheta;


  // get the error
  error = theta - masterTheta;

  // if the error is positive the motor should be going forward
  // if the error is negative the motor should be going backward 
  if (error > 0){
    digitalWrite(motorLDir, HIGH);
  } else {
    digitalWrite(motorLDir, LOW);
  }

  // we don't want a negative error for our calculations
  error = abs(error);

  // see if were close enough to the target to shut off control
  if (error < shutOffError){
    // assign the current time
    currentTime = millis();
    if (currentTime > turnDoneTime + deltaDone) {
      // set flag
      turnDone = true;
    }
    // dont deliver anything
    analogWrite(motorLVolt, 0);


  } else {
    // set flag
    turnDone = false;

    // deliver something
    // check for windup tolerance
    if (error > windUpTolerance){
      I = 0;
      voltage = error * Kp;
      Serial.println("dont integrate");
    } else {
      // use integrator
      I = I + error * ((time_start - time_past)/1000);
      voltage = (error * Kp) + (Ki * I);
      Serial.println(I);
    }

    // check if the voltage is too high
    if (voltage > maxVoltage){
      voltage = maxVoltage;
    }

    // deliver power to the motor
    analogWrite(motorLVolt, round(abs(voltage/maxVoltage) * MAX_PWM));

    // assign time
    turnDoneTime = millis();
  }

  // return the masters voltage
  return voltage;
}

/*
  void turn_slave(float masterTheta, float slaveThetat, float desiredTheta, float masterVolt, float time_start, float time_past)
  PARAMETERS:
  RETURN:
  FUNCTIONALITY:
*/
void turn_slave(float masterTheta, float slaveTheta, float desiredTheta, float masterVolt, float time_start, float time_past){
  // define variables needed
  float error_right_turn;
  float error_left_turn;
  float error;
  float voltage = 0;

  // calculate the error off the master
  error_right_turn = masterTheta - abs(slaveTheta);
  error_left_turn = abs(masterTheta) - slaveTheta;

  // print both of the errors
  // Serial.print(error_left_turn);
  // Serial.print("\t");
  // Serial.print(error_right_turn);
  // Serial.println();

  // assign the direction it needs to turn
  // desiredTheta > 0 -> turn right -> have R LOW
  // desiredTheta < 0 -> turn left
  if (desiredTheta > 0){
    if (error_right_turn > 0){
      digitalWrite(motorRDir, LOW);
    } else {
      digitalWrite(motorRDir, HIGH);
    }
  } else {
    if (error_left_turn > 0) {
      digitalWrite(motorRDir, HIGH);
    } else {
      digitalWrite(motorRDir, LOW);
    }
  }

  // dont want the error to be negative for the calculations
  error = abs(abs(masterTheta) - abs(slaveTheta));

  // see if its close enough to our desired location
  if (error < shutOffError){
    analogWrite(motorRVolt, 0);

  } else {

    // check if wind up ie check to use integrator
    if (error > windUpTolerance){
      I_slave = 0;
      voltage = error * Kp_slave;
    } else {
      // use integrator
      I_slave = I_slave + error * ((time_start - time_past)/1000);
      voltage = (error * Kp_slave) + (Ki_slave * I_slave);
    }
    
    // add the voltage to the masterVolt
    voltage = voltage + masterVolt;

    // check if the voltage is too high
    if (voltage > maxVoltage){
      voltage = maxVoltage;
    }

    // drive the motor 
    analogWrite(motorRVolt, round(abs(voltage/maxVoltage) * MAX_PWM));
  }
}
