#include <SoftwareSerial.h>
#include "EasyTransfer.h"
#include <Wire.h>

const int FORWARD_CHARACTER = 0;
const int REVERSE_CHARACTER = 1;

#define MAX_LOG_LENGTH 100 //Maximum number of characters in a log print
//when sending data to the motherboard. Limited due to limitations of 
//EasyTransfer library. Extra space will be padding with crap (null-terminated strings)


//----------------------------------------------
//Easy Transfer stuff
EasyTransfer ETsend;
EasyTransfer ETreceive;

struct SEND_DATA_STRUCTURE{
  char log_data[MAX_LOG_LENGTH];
};

struct RECEIVE_DATA_STRUCTURE{
  boolean joint1Forward;
  boolean joint1Backward;
  boolean joint2Forward;
  boolean joint2Backward;
  boolean joint3Forward;
  boolean joint3Backward;
  boolean joint4Forward;
  boolean joint4Backward;
  boolean joint5Forward;
  boolean joint5Backward;
  boolean joint6Forward;
  boolean joint6Backward;
};

SEND_DATA_STRUCTURE sendData;
RECEIVE_DATA_STRUCTURE receiveData;

boolean DEBUG_MODE = true;
const int DEBUG_MODE_SELECT = A5;

void setup()
{
  Serial.begin(9600);
  if(!DEBUG_MODE)
  {
    //initialize easy transfer object
    receiveData.joint1Forward = false;
    receiveData.joint1Backward = false;
    receiveData.joint2Forward = false;
    receiveData.joint2Backward = false;
    receiveData.joint3Forward = false;
    receiveData.joint3Backward = false;
    receiveData.joint4Forward = false;
    receiveData.joint4Backward = false;
    receiveData.joint5Forward = false;
    receiveData.joint5Backward = false;
    receiveData.joint6Forward = false;
    receiveData.joint6Backward = false;
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

void loop()
{     
  if(!DEBUG_MODE && ETreceive.receiveData())
  {
    // Not DEBUG MODE version
    // Go through each part of the receive struct.
    
    if(receiveData.joint1Forward){
      Wire.beginTransmission(1);
      Wire.write(FORWARD_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint1Backward){
      Wire.beginTransmission(1);
      Wire.write(REVERSE_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint2Forward){
      Wire.beginTransmission(2);
      Wire.write(FORWARD_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint2Backward){
      Wire.beginTransmission(2);
      Wire.write(REVERSE_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint3Forward){
      Wire.beginTransmission(3);
      Wire.write(FORWARD_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint3Backward){
      Wire.beginTransmission(3);
      Wire.write(REVERSE_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint4Forward){
      Wire.beginTransmission(4);
      Wire.write(FORWARD_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint4Backward){
      Wire.beginTransmission(4);
      Wire.write(REVERSE_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint5Forward){
      Wire.beginTransmission(5);
      Wire.write(FORWARD_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint5Backward){
      Wire.beginTransmission(5);
      Wire.write(REVERSE_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint6Forward){
      Wire.beginTransmission(6);
      Wire.write(FORWARD_CHARACTER);
      Wire.endTransmission();
    }
    if(receiveData.joint6Backward){
      Wire.beginTransmission(6);
      Wire.write(REVERSE_CHARACTER);
      Wire.endTransmission();
    }
    
    // Reset receive struct for next round
    receiveData.joint1Forward = false;
    receiveData.joint1Backward = false;
    receiveData.joint2Forward = false;
    receiveData.joint2Backward = false;
    receiveData.joint3Forward = false;
    receiveData.joint3Backward = false;
    receiveData.joint4Forward = false;
    receiveData.joint4Backward = false;
    receiveData.joint5Forward = false;
    receiveData.joint5Backward = false;
    receiveData.joint6Forward = false;
    receiveData.joint6Backward = false;
  }
  else if(DEBUG_MODE && Serial.available()>0)
  {
    //DEBUG MODE version
    byte received = Serial.read();//FIXME different if using struct serializer
    printLog(received);
    switch(received)
    {
      //left belt
      case 'q':
        Wire.beginTransmission(1);
        Wire.write(FORWARD_CHARACTER);
        Wire.endTransmission();
        
        break;
      case 'a':
        Wire.beginTransmission(1);
        Wire.write(REVERSE_CHARACTER);
        Wire.endTransmission();
        break;
        
      //right belt
      case 'w':
        Wire.beginTransmission(2);
        Wire.write(FORWARD_CHARACTER);
        Wire.endTransmission();
        break;
      case 's':
        Wire.beginTransmission(2);
        Wire.write(REVERSE_CHARACTER);
        Wire.endTransmission();
        break;
        
      //linear actuator
      case 'e':
        Wire.beginTransmission(3);
        Wire.write(FORWARD_CHARACTER);
        Wire.endTransmission();
        break;
      case 'd':
        Wire.beginTransmission(3);
        Wire.write(REVERSE_CHARACTER);
        Wire.endTransmission();
        break;
      
      //left small differential
      case 'r':
        Wire.beginTransmission(4);
        Wire.write(FORWARD_CHARACTER);
        Wire.endTransmission();
        break;
      case 'f':
        Wire.beginTransmission(4);
        Wire.write(REVERSE_CHARACTER);
        Wire.endTransmission();
        break;
        
      //right small differential
      case 't':
        Wire.beginTransmission(5);
        Wire.write(FORWARD_CHARACTER);
        Wire.endTransmission();
        break;
      case 'g':
        Wire.beginTransmission(5);
        Wire.write(REVERSE_CHARACTER);
        Wire.endTransmission();
        break;
        
      //base servo
      case 'y':
        Wire.beginTransmission(6);
        Wire.write(FORWARD_CHARACTER);
        Wire.endTransmission();
        break;
      case 'h':
        Wire.beginTransmission(6);
        Wire.write(REVERSE_CHARACTER);
        Wire.endTransmission();
        break;
        
      default:
        printLog("Invalid");
    }
  }
}

void printLog(String s)
{
  if(DEBUG_MODE)
  {
    Serial.println(s);
  }
  else
  {
    //TODO: Struct serializer log
  }
}

void printLog(char c)
{
  String s(c);
  printLog(s);
}
