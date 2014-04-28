//#include "TimerOne.h"
//#include "FrequencyTimer2.h" //Library for Controller Interrupt on Timer2
#include <avr/io.h>
#include <stdint.h>
float voltage = 0;
float temperature = 0;
float current = 0;
volatile int cposition = 0;
int setpointposition = 0;
int error = 0;
volatile int pwm = 0;
int integrator = 0;
byte incomingbyte = 0;

const int TIME_DELAY = 500;
 
//Motor tuning parameters
const int ki = 1;
const int kp = 4;
 
void setup(){
// Setting up pins in proper mode
pinMode(5, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);
pinMode(11, OUTPUT);
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);
pinMode(A0, OUTPUT);
pinMode(A1, OUTPUT);
// Begin Serial
Serial.begin(9600);
 
//Configurating the motor driver
digitalWrite(13, HIGH); //Enable PCMC
digitalWrite(12, LOW);
digitalWrite(8, LOW); //Set up DCM operation
digitalWrite(10, HIGH); //DCM mode in manual PWM operation
digitalWrite(11, LOW); //Short blanking
digitalWrite(A0, HIGH);//Going forward
digitalWrite(A1, LOW);//Initially the motor is off
//Serial.println("Starting...");
 
}
 
void loop(){// Main loop executing non-time sensitive code
  if(Serial.available()>0){
    incomingbyte = Serial.
    read();
    if(incomingbyte == 'q')
    {
      digitalWrite(A1, HIGH);
      digitalWrite(A0,HIGH);
      digitalWrite(9,HIGH);
      delay(TIME_DELAY);
      digitalWrite(9,LOW);
      digitalWrite(A1, LOW);
    }
    else if(incomingbyte == 'a')
    {
      digitalWrite(A1, HIGH);
      digitalWrite(A0,LOW);
      digitalWrite(9,HIGH);
      delay(TIME_DELAY);
      digitalWrite(9,LOW);
      digitalWrite(A1,LOW);
    }
  }
}
