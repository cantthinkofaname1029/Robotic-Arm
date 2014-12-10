/*
 * Programmer: Drue satterfield      date: 12/10/14  file: armV3.c
 *
 */
/*
#include "armV3.h"
//
// The instance data for the software UART.
//
tSoftUART g_sUART;
//
// The buffer used to hold the transmit data.
//
unsigned char g_pucTxBuffer[16];
//
// The buffer used to hold the receive data.
//
unsigned short g_pusRxBuffer[16];
//
// The number of processor clocks in the time period of a single bit on the
// software UART interface.
//
unsigned long g_ulBitTime;
//
// The transmit timer tick function.
//
void
Timer0AIntHandler(void)
{
	//
	// Clear the timer interrupt.
	//
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	//
	// Call the software UART transmit timer tick function.
	//
	SoftUARTTxTimerTick(&g_sUART);
}

//
// The receive timer tick function.
//
void
Timer0BIntHandler(void)
{
	//
	// Clear the timer interrupt.
	//
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	//
	// Call the software UART receive timer tick function, and see if the
	// timer should be disabled.
	//
	if(SoftUARTRxTick(&g_sUART, false) == SOFTUART_RXTIMER_END)
	{
		//
		// Disable the timer interrupt since the software UART doesn’t need
		// it any longer.
		//
		TimerDisable(TIMER0_BASE, TIMER_B);
	}
}

//
// The interrupt handler for the software UART GPIO edge interrupt.
//
void GPIOIntHandler(void)
{
	//
	// Configure the software UART receive timer so that it samples at the
	// mid-bit time of this character.
	//
	TimerDisable(TIMER0_BASE, TIMER_B);
	TimerLoadSet(TIMER0_BASE, TIMER_B, g_ulBitTime);
	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_B);
	//
	// Call the software UART receive timer tick function.
	//
	SoftUARTRxTick(&g_sUART, true);
}
//
// The callback function for the software UART. This function is
// equivalent to the interrupt handler for a hardware UART.
//
void UARTCallback(void)
{
	unsigned long ulInts;
	//
	// Read the asserted interrupt sources.
	//
	ulInts = SoftUARTIntStatus(&g_sUART, true);
	//
	// Clear the asserted interrupt sources.
	//
	SoftUARTIntClear(&g_sUART, ulInts);
	//
	// Handle the asserted interrupts.
	//
	//todo: add more stuff if we want to utilize more interrupts
}


void initialize(void)
{
	//
	// Initialize the software UART instance data.
	//
	SoftUARTInit(&g_sUART);

	//todo: assign the system clocks how we wish
	//
	// Enable GPIO port A which is used for UART0 pins.
	// TODO: change this to whichever GPIO port you are using.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // For this example, Timer0 is used for the SoftUART time base.  This timer
    // needs to be enabled before it can be used.
    // TODO: change this to whichever timer you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //switch the pins to their UART functionality
    //todo: figure out what the actual pins are
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);

	//
	// Set the callback function used for this software UART.
	//
	SoftUARTCallbackSet(&g_sUART, UARTCallback);
	//
	// Configure the pins used for the software UART. This example uses
	// pins PD0 and PE1.
	//
	SoftUARTTxGPIOSet(&g_sUART, GPIO_PORTD_BASE, GPIO_PIN_0);
	SoftUARTRxGPIOSet(&g_sUART, GPIO_PORTE_BASE, GPIO_PIN_1);
	//
	// Configure the data buffers used as the transmit and receive buffers.
	//
	SoftUARTTxBufferSet(&g_sUART, g_pucTxBuffer, 16);
	SoftUARTRxBufferSet(&g_sUART, g_pusRxBuffer, 16);
	//
	// Enable the GPIO modules that contains the GPIO pins to be used by
	// the software UART.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	//
	// Configure the software UART module: 8 data bits, no parity, and one
	// stop bit.
	//
	SoftUARTConfigSet(&g_sUART,
	(SOFTUART_CONFIG_WLEN_8 | SOFTUART_CONFIG_PAR_NONE |
	SOFTUART_CONFIG_STOP_ONE));
	//
	// Compute the bit time for 38,400 baud.
	//
	g_ulBitTime = (SysCtlClockGet() / 38400) - 1;
	//
	// Configure the timers used to generate the timing for the software
	// UART. The interface in this example is run at 38,400 baud,
	// requiring a timer tick at 38,400 Hz.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE,
	(TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_PERIODIC |
	TIMER_CFG_B_PERIODIC));
	TimerLoadSet(TIMER0_BASE, TIMER_A, g_ulBitTime);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_A);
	//
	// Set the priorities of the interrupts associated with the software
	// UART. The receiver is higher priority than the transmitter, and the
	// receiver edge interrupt is higher priority than the receiver timer
	// interrupt.
	//
	IntPrioritySet(INT_GPIOE, 0x00);
	IntPrioritySet(INT_TIMER0B, 0x40);
	IntPrioritySet(INT_TIMER0A, 0x80);
	//
	// Enable the interrupts associated with the software UART.
	//
	IntEnable(INT_GPIOE);
	IntEnable(INT_TIMER0A);
	IntEnable(INT_TIMER0B);
	//
	// Enable the transmit FIFO half full interrupt in the software UART.
	//
	SoftUARTIntEnable(&g_sUART, SOFTUART_INT_TX);
}

int main(void){
	int * debug;
	//necessary variable definitions
	const int FORWARD_CHARACTER = 0;
	const int REVERSE_CHARACTER = 1;
	int last = 0;
	int count = 0;

	volatile int state = 0; //unsure if this will work


	typedef struct {
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
	}RECEIVE_DATA_STRUCTURE;

	RECEIVE_DATA_STRUCTURE receiveData;

	short int DEBUG_MODE = false;
	//const int DUBUG_MODE_SELECT = A3; //unsure if A3 is an int.

	//setup

	//Serial.begin(115200);
	    resetStruct();
	    //may or may not need the Wire.begin();
	  //  Wire.begin(); // join i2c bus (address optional for master)

	//while loop for running the main program
	while(1)
	{
		if(ETreceive.receiveData()){
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
  write(FORWARD_CHARACTER); //psudeo funciton.
  delay(DELAY);
  last=1;
}

void wristCOunterClockWise(){
	write(REVERSE_CHARACTER);
	delay(DELAY);
	last=2;
}

void wristUp(){
	write(FORWARD_CHARACTER);
	delay(DELAY);
	last=3;
}

void wristDown(){
	write(REVERSE_CHARACTER);
	delay(DELAY);
	last=4;
}

void elbowCounterClockWise(){
	write(FORWARD_CHARACTER);
	delay(DELAY);
	last=5;
}

void elbowClockWise(){
	write(REVERSE_CHARACTER);
	delay(DELAY);
	last=6;
}

void elbowDown(){
	write(FORWARD_CHARACTER);
	delay(DELAY);
	last=7;
}

void elbowUp(){
	write(REVERSE_CHARACTER);
	delay(DELAY);
	last=8;
}

void actuatorForward(){
	write(FORWARD_CHARACTER);
	delay(DELAY);
	last=9;
}

void actuatorReverse(){
	write(REVERSE_CHARACTER);
	delay(DELAY);
	last=10;
}

void baseClockWise(){
	write(FORWARD_CHARACTER);
	delay(DELAY);
	last=11;
}

void baseCounterClockWise(){
	write(REVERSE_CHARACTER);
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
*/
