#include <Wire.h>

int wireAddress=4;

float voltage = 0;
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
  Wire.begin(wireAddress);      // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);   
  
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

  //Configurating the motor driver
  digitalWrite(13, HIGH); //Enable PCMC
  digitalWrite(12, LOW);
  digitalWrite(8, LOW); //Set up DCM operation
  digitalWrite(10, HIGH); //DCM mode in manual PWM operation
  digitalWrite(11, LOW); //Short blanking
  digitalWrite(A0, HIGH);//Going forward
  digitalWrite(A1, LOW);//Initially the motor is off
}
 
void loop(){// Main loop executing non-time sensitive code
  delay(100);
}

void receiveEvent(int howMany)
{
  int incomingbyte = Wire.read();    // receive byte as an integer
  if(incomingbyte == 0)
  {
    Serial.println("0");
    digitalWrite(A1, HIGH);
    digitalWrite(A0,HIGH);
    digitalWrite(9,HIGH);
    delay(TIME_DELAY);
    digitalWrite(9,LOW);
    digitalWrite(A1, LOW);
  }
  else if(incomingbyte == 1)
  {
    Serial.println("1");
    digitalWrite(A1, HIGH);
    digitalWrite(A0,LOW);
    digitalWrite(9,HIGH);
    delay(TIME_DELAY);
    digitalWrite(9,LOW);
    digitalWrite(A1,LOW);
  }
}
