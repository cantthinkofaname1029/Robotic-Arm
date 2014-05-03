#include "SoftwareSerial.h"
#include "EasyTransfer.h"

#define FORWARD_CHARACTER 'q'
#define REVERSE_CHARACTER 'a'

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


//----------------------------------------------------
// motor controller serial declerations
SoftwareSerial serial1(3,2);//rx,tx
SoftwareSerial serial2(5,4);
SoftwareSerial serial3(7,6);
SoftwareSerial serial4(9,8);
SoftwareSerial serial5(11,10);
SoftwareSerial serial6(13,12);

boolean DEBUG_MODE = false;
const int DEBUG_MODE_SELECT = A5;

void setup()
{
  pinMode(DEBUG_MODE_SELECT, INPUT);
  DEBUG_MODE = digitalRead(DEBUG_MODE_SELECT);
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
  serial1.begin(9600);
  serial2.begin(9600);
  serial3.begin(9600);
  serial4.begin(9600);
  serial5.begin(9600);
  serial6.begin(9600);
  printLog("Software Serial Initialized");
}

void loop()
{
  if(Serial.available()>0)
  {
    byte received = Serial.read();//FIXME different if using struct serializer
    printLog(received);
    switch(received)
    {
      //left belt
      case 'q':
        serial1.write(FORWARD_CHARACTER);
        break;
      case 'a':
        serial1.write(REVERSE_CHARACTER);
        break;
        
      //right belt
      case 'w':
        serial2.write(FORWARD_CHARACTER);
        break;
      case 's':
        serial2.write(REVERSE_CHARACTER);
        break;
        
      //linear actuator
      case 'e':
        serial3.write(FORWARD_CHARACTER);
        break;
      case 'd':
        serial3.write(REVERSE_CHARACTER);
        break;
      
      //left small differential
      case 'r':
        serial4.write(FORWARD_CHARACTER);
        break;
      case 'f':
        serial4.write(REVERSE_CHARACTER);
        break;
        
      //right small differential
      case 't':
        serial5.write(FORWARD_CHARACTER);
        break;
      case 'g':
        serial5.write(REVERSE_CHARACTER);
        break;
        
      //base servo
      case 'y':
        serial6.write(FORWARD_CHARACTER);
        break;
      case 'h':
        serial6.write(REVERSE_CHARACTER);
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
