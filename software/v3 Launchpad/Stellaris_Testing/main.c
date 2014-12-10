#include <lm4f120h5qr.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#define DELAY 5
#define BYTE

struct RECEIVE_DATA_STRUCTURE{
	  unsigned int reset;
	  unsigned int wristUp;
	  unsigned int wristDown;
	  unsigned int wristClockWise;
	  unsigned int wristCounterClockWise;
	  unsigned int elbowUp;
	  unsigned int elbowDown;
	  unsigned int elbowClockWise;
	  unsigned int elbowCounterClockWise;
	  unsigned int actuatorForward;
	  unsigned int actuatorReverse;
	  unsigned int baseClockWise;
	  unsigned int baseCounterClockWise;
	};

void reset(void);

int main(void){
	//necessary variable definitions
	const int FORWARD_CHARACTER = 0;
	const int REVERSE_CHARACTER = 1;
	unsigned int last = 0;
	unsigned int count = 0;
	volatile int state = 0; //unsure if this will work
	struct RECEIVE_DATA_STRUCTURE receiveData;
	short int DEBUG_MODE = false;

	//const int DUBUG_MODE_SELECT = A3; //unsure if A3 is an int.
	//setup
	UARTStdioInit(0);
	resetStruct();
	//while loop for running the main program
	while(1)
	{
		if(UARTgets()){
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
	}

}

void wristClockWise(){
  UARTprintf(FORWARD_CHARACTER); //psudeo funciton.
  delay(DELAY);
  last=1;
}

void wristCOunterClockWise(){
	UARTprintf(REVERSE_CHARACTER);
	delay(DELAY);
	last=2;
}

void wristUp(){
	UARTprintf(FORWARD_CHARACTER);
	delay(DELAY);
	last=3;
}

void wristDown(){
	UARTprintf(REVERSE_CHARACTER);
	delay(DELAY);
	last=4;
}

void elbowCounterClockWise(){
	UARTprintf(FORWARD_CHARACTER);
	delay(DELAY);
	last=5;
}

void elbowClockWise(){
	UARTprintf(REVERSE_CHARACTER);
	delay(DELAY);
	last=6;
}

void elbowDown(){
	UARTprintf(FORWARD_CHARACTER);
	delay(DELAY);
	last=7;
}

void elbowUp(){
	UARTprintf(REVERSE_CHARACTER);
	delay(DELAY);
	last=8;
}

void actuatorForward(){
	UARTprintf(FORWARD_CHARACTER);
	delay(DELAY);
	last=9;
}

void actuatorReverse(){
	UARTprintf(REVERSE_CHARACTER);
	delay(DELAY);
	last=10;
}

void baseClockWise(){
	UARTprintf(FORWARD_CHARACTER);
	delay(DELAY);
	last=11;
}

void baseCounterClockWise(){
	UARTprintf(REVERSE_CHARACTER);
	delay(DELAY);
	last=12;
}
//resets all components to the struct.
void resetStruct(){
  receiveData.reset = 0;
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
