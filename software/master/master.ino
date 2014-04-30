#include <SoftwareSerial.h>
#define FORWARD_CHARACTER 'q'
#define REVERSE_CHARACTER 'a'

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
