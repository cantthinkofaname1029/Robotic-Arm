#include "EasyTransfer.h"
#include <Wire.h>

const int FORWARD_CHARACTER = 0;
const int REVERSE_CHARACTER = 1;
int last = 0;
int count = 0;

volatile int state = LOW;

#define MAX_LOG_LENGTH 100

EasyTransfer ETsend;
EasyTransfer ETreceive;

struct SEND_DATA_STRUCTURE{
  char log_data[MAX_LOG_LENGTH];
};

struct RECEIVE_DATA_STRUCTURE{
  uint8_t reset;
  uint8_t wristUp;
  uint8_t wristDown;
  uint8_t wristClockWise;
  uint8_t wristCounterClockWise;
  uint8_t elbowUp;
  uint8_t elbowDown;
  uint8_t elbowClockWise;
  uint8_t elbowCounterClockWise;
  uint8_t actuatorForward;
  uint8_t actuatorReverse;
  uint8_t baseClockWise;
  uint8_t baseCounterClockWise;
};

SEND_DATA_STRUCTURE sendData;
RECEIVE_DATA_STRUCTURE receiveData;

boolean DEBUG_MODE = false;
const int DEBUG_MODE_SELECT=A3;

void setup(){
  // Setup reset pin
  pinMode(4, OUTPUT);
  
  attachInterrupt(0, backup, RISING);
  if(analogRead(DEBUG_MODE_SELECT)<=10){
    DEBUG_MODE = true;
    Serial.begin(9600);
  }
  else{
    Serial.begin(115200);
    resetStruct();
    sendData.log_data[0]='\0';
    ETsend.begin(details(sendData), &Serial);
    ETreceive.begin(details(receiveData), &Serial);
  }
  
  printLog("Robotic Arm Initializing");
  printLog(DEBUG_MODE ? "Robotic Arm in Debug Mode" : "Robotic Arm in Operating Mode");
  printLog("Hardware Serial Initialized");
  Wire.begin(); // join i2c bus (address optional for master)
  printLog("I2C Initilaized");
  printLog("Hello, this is Robotic Arm. (Robotic Arm Ready)");
}

void loop(){
  if(!DEBUG_MODE && ETreceive.receiveData() && count<=50){ 
    count++;
    if(receiveData.reset)
      reset();
    if(receiveData.wristUp)
      wristUp();
    if(receiveData.wristDown)
      wristDown();
    if(receiveData.wristClockWise)
      wristClockWise();
    if(receiveData.wristCounterClockWise)
      wristCounterClockWise();
    if(receiveData.elbowUp)
      elbowUp();
    if(receiveData.elbowDown)
      elbowDown();
    if(receiveData.elbowClockWise)
      elbowClockWise();
    if(receiveData.elbowCounterClockWise)
      elbowCounterClockWise();
    if(receiveData.actuatorForward)
      actuatorForward();
    if(receiveData.actuatorReverse)
      actuatorReverse();
    if(receiveData.baseClockWise)
      baseClockWise();
    if(receiveData.baseCounterClockWise)
      baseCounterClockWise();
    resetStruct();
  }
  else if(DEBUG_MODE && Serial.available()>0 && count<=50){
    Serial.println("Serial mode");
    byte received = Serial.read();
    printLog(received);
    count++;
    switch(received){
      case 'w':
        wristUp();
        break;
      case 's':
        wristDown();
        break;
      case 'a':
        wristClockWise();
        break;
      case 'd':
        wristCounterClockWise();
        break;
      case 'i':
        elbowUp();
        break;
      case 'k':
        elbowDown();
        break;
      case 'j':
        elbowClockWise();
        break;
      case 'l':
        elbowCounterClockWise();
        break;
      case 't':
        actuatorForward();
        break;
      case 'g':
        actuatorReverse();
        break;
      case 'f':
        baseClockWise();
        break;
      case 'h':
        baseCounterClockWise();
        break;
      default:
        printLog("Invalid");
    }
  }
  else if(count>0){
    Serial.println("Overload");
    int x=count;
    for(int i=0; i<x; i++){
      delay(5);
      count--;
    } 
  }
}

void printLog(String s){
  if(DEBUG_MODE)
    Serial.println(s);
}

void printLog(char c){
  String s(c);
  printLog(s);
}

void reset(){
  digitalWrite(4, LOW);
  delay(2000);
  pinMode(4, INPUT);
}

void wristClockWise(){
  Wire.beginTransmission(1);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  Wire.beginTransmission(2);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  last=1;
}

void wristCounterClockWise(){
  Wire.beginTransmission(1);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  Wire.beginTransmission(2);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  last=2;
}

void wristUp(){
  Wire.beginTransmission(1);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  Wire.beginTransmission(2);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  last=3;
}

void wristDown(){
  Wire.beginTransmission(1);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  Wire.beginTransmission(2);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  last=4;
}

void elbowCounterClockWise(){
  Wire.beginTransmission(3);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  Wire.beginTransmission(4);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  last=5;
}

void elbowClockWise(){
  Wire.beginTransmission(3);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  Wire.beginTransmission(4);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  last=6;
}

void elbowDown(){
  Wire.beginTransmission(3);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  Wire.beginTransmission(4);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  last=7;
}

void elbowUp(){
  Wire.beginTransmission(3);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  Wire.beginTransmission(4);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  last=8;
}

void actuatorForward(){
  Wire.beginTransmission(5);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  last=9;
}

void actuatorReverse(){
  Wire.beginTransmission(5);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  last=10;
}

void baseClockWise(){
  Wire.beginTransmission(6);
  Wire.write(FORWARD_CHARACTER);
  Wire.endTransmission();
  last=11;
}

void baseCounterClockWise(){
  Wire.beginTransmission(6);
  Wire.write(REVERSE_CHARACTER);
  Wire.endTransmission();
  last=12;
}

void resetStruct(){
  receiveData.wristUp = 0;
  receiveData.wristDown = 0;
  receiveData.wristClockWise = 0;
  receiveData.wristCounterClockWise = 0;
  receiveData.elbowUp = 0;
  receiveData.elbowDown = 0;
  receiveData.elbowClockWise = 0;
  receiveData.elbowCounterClockWise = 0;
  receiveData.actuatorForward = 0;
  receiveData.actuatorReverse = 0;
  receiveData.baseClockWise = 0;
  receiveData.baseCounterClockWise = 0; 
}

void backup(){
  Serial.println("Backup");
  if(last==2)
    wristUp();
  if(last==1)
    wristDown();
  if(last==4)
    wristClockWise();
  if(last==3)
    wristCounterClockWise();
  if(last==6)
    elbowUp();
  if(last==5)
    elbowDown();
  if(last==8)
    elbowClockWise();
  if(last==7)
    elbowCounterClockWise();
  if(last==10)
    actuatorForward();
  if(last==9)
    actuatorReverse();
  if(last==12)
    baseClockWise();
  if(last==11)
    baseCounterClockWise();
  delay(1000);
}
