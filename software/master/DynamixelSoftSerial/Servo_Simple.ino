#include <DynamixelSoftSerial.h>
#include <SoftwareSerial.h>

int num = 200;
void setup()
{
Dynamixel.begin(57600,2,3,4);  // Inicialize the servo at 1Mbps and Pin Control 2
delay(1000);
Serial.begin(9600);
}

void loop()
{
  int myRead=Serial.read();
  Serial.println(myRead);
  if(myRead==121)
    turnRight();
  if(myRead==104)
    turnLeft();
}


/*void loop() 
{
  int myRead=Serial.read();
  Serial.println(myRead);
  if(myRead==119)
  {
    Dynamixel.setEndless(1,ON);
    Dynamixel.turn(1,RIGTH,500);
    delay(500);
    Dynamixel.turn(1,RIGTH,0);
  }
  if(myRead==115)
  {
    Dynamixel.setEndless(1,ON);
    Dynamixel.turn(1,LEFT,500);
    delay(400);
    Dynamixel.turn(1,LEFT,0);
  }
}*/

void turnRight()
{
  Dynamixel.setEndless(1,ON);
  Dynamixel.turn(1,RIGTH,500);
  delay(500);
  Dynamixel.turn(1,RIGTH,0);
}

void turnLeft()
{
  Dynamixel.setEndless(1,ON);
  Dynamixel.turn(1,RIGTH,500);
  delay(500);
  Dynamixel.turn(1,RIGTH,0);
}
