#include "armBoard.h"

//*****************************************************************************
//
// The state of the SoftI2C module.
//
//*****************************************************************************
static tSoftI2C g_sI2C;

//*****************************************************************************
//
// The variables that track the data to be transmitted or received.
//
//*****************************************************************************
static unsigned char *g_pucData = 0;
static unsigned long g_ulCount = 0;

//*****************************************************************************
//
// The current state of the interrupt handler state machine.
//
//*****************************************************************************
static volatile unsigned long g_ulState = STATE_IDLE;

void
InitConsole(void)
{
	//todo: assign the system clocks how we wish
		//
		// Enable GPIO port A which is used for UART0 pins.
		// TODO: change this to whichever GPIO port you are using.
		//
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);


       // For this example, PortB[3:2] are used for the SoftI2C pins.  GPIO port B
       // needs to be enabled so these pins can be used.
       // TODO: change this to whichever GPIO port(s) you are using.
       //
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

       //
       // For this example, Timer0 is used for the SoftI2C time base.  This timer
       // needs to be enabled before it can be used.
       // TODO: change this to whichever timer you are using.
       //
       SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

       //
       // Configure the appropriate pins to be I2C instead of GPIO.
       //
       GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);

       //
       // Initialize the SoftI2C module, including the assignment of GPIO pins.
       // TODO: change this to whichever GPIO pins you are using.
       //
       memset(&g_sI2C, 0, sizeof(g_sI2C));
       SoftI2CCallbackSet(&g_sI2C, SoftI2CCallback);
       SoftI2CSCLGPIOSet(&g_sI2C, GPIO_PORTB_BASE, GPIO_PIN_2);
       SoftI2CSDAGPIOSet(&g_sI2C, GPIO_PORTB_BASE, GPIO_PIN_3);
       SoftI2CInit(&g_sI2C);

       //
       // Enable the SoftI2C interrupt.
       //
       SoftI2CIntEnable(&g_sI2C);

       //
       // Configure the timer to generate an interrupt at a rate of 40 KHz.  This
       // will result in a I2C rate of 10 KHz.
       // TODO: change this to whichever timer you are using.
       // TODO: change this to whichever I2C rate you require.
       //
       TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
       TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 40000);
       TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
       TimerEnable(TIMER0_BASE, TIMER_A);

       //
       // Enable the timer interrupt.
       // TODO: change this to whichever timer interrupt you are using.
       //
       IntEnable(INT_TIMER0A);



}

//*****************************************************************************
//
// The callback function for the SoftI2C module.
//
//*****************************************************************************
void SoftI2CCallback(void)
{
    //
    // Clear the SoftI2C interrupt.
    //
    SoftI2CIntClear(&g_sI2C);

    //
    // Determine what to do based on the current state.
    //
    switch(g_ulState)
    {
        //
        // The idle state.
        //
        case STATE_IDLE:
        {
            //
            // There is nothing to be done.
            //
            break;
        }

        //
        // The state for the middle of a burst write.
        //
        case STATE_WRITE_NEXT:
        {
            //
            // Write the next data byte.
            //
            SoftI2CDataPut(&g_sI2C, *g_pucData++);
            g_ulCount--;

            //
            // Continue the burst write.
            //
            SoftI2CControl(&g_sI2C, SOFTI2C_CMD_BURST_SEND_CONT);

            //
            // If there is one byte left, set the next state to the final write
            // state.
            //
            if(g_ulCount == 1)
            {
                g_ulState = STATE_WRITE_FINAL;
            }

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for the final write of a burst sequence.
        //
        case STATE_WRITE_FINAL:
        {
            //
            // Write the final data byte.
            //
            SoftI2CDataPut(&g_sI2C, *g_pucData++);
            g_ulCount--;

            //
            // Finish the burst write.
            //
            SoftI2CControl(&g_sI2C, SOFTI2C_CMD_BURST_SEND_FINISH);

            //
            // The next state is to wait for the burst write to complete.
            //
            g_ulState = STATE_SEND_ACK;

            //
            // This state is done.
            //
            break;
        }

        //
        // Wait for an ACK on the read after a write.
        //
        case STATE_WAIT_ACK:
        {
            //
            // See if there was an error on the previously issued read.
            //
            if(SoftI2CErr(&g_sI2C) == SOFTI2C_ERR_NONE)
            {
                //
                // Read the byte received.
                //
                SoftI2CDataGet(&g_sI2C);

                //
                // There was no error, so the state machine is now idle.
                //
                g_ulState = STATE_IDLE;

                //
                // This state is done.
                //
                break;
            }

            //
            // Fall through to STATE_SEND_ACK.
            //
        }

        //
        // Send a read request, looking for the ACK to indicate that the write
        // is done.
        //
        case STATE_SEND_ACK:
        {
            //
            // Put the I2C master into receive mode.
            //
            SoftI2CSlaveAddrSet(&g_sI2C, SLAVE_ADDR, true);

            //
            // Perform a single byte read.
            //
            SoftI2CControl(&g_sI2C, SOFTI2C_CMD_SINGLE_RECEIVE);

            //
            // The next state is the wait for the ack.
            //
            g_ulState = STATE_WAIT_ACK;

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for a single byte read.
        //
        case STATE_READ_ONE:
        {
            //
            // Put the SoftI2C module into receive mode.
            //
            SoftI2CSlaveAddrSet(&g_sI2C, SLAVE_ADDR, true);

            //
            // Perform a single byte read.
            //
            SoftI2CControl(&g_sI2C, SOFTI2C_CMD_SINGLE_RECEIVE);

            //
            // The next state is the wait for final read state.
            //
            g_ulState = STATE_READ_WAIT;

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for the start of a burst read.
        //
        case STATE_READ_FIRST:
        {
            //
            // Put the SoftI2C module into receive mode.
            //
            SoftI2CSlaveAddrSet(&g_sI2C, SLAVE_ADDR, true);

            //
            // Start the burst receive.
            //
            SoftI2CControl(&g_sI2C, SOFTI2C_CMD_BURST_RECEIVE_START);

            //
            // The next state is the middle of the burst read.
            //
            g_ulState = STATE_READ_NEXT;

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for the middle of a burst read.
        //
        case STATE_READ_NEXT:
        {
            //
            // Read the received character.
            //
            *g_pucData++ = SoftI2CDataGet(&g_sI2C);
            g_ulCount--;

            //
            // Continue the burst read.
            //
            SoftI2CControl(&g_sI2C, SOFTI2C_CMD_BURST_RECEIVE_CONT);

            //
            // If there are two characters left to be read, make the next
            // state be the end of burst read state.
            //
            if(g_ulCount == 2)
            {
                g_ulState = STATE_READ_FINAL;
            }

            //
            // This state is done.
            //
            break;
        }

        //
        // The state for the end of a burst read.
        //
        case STATE_READ_FINAL:
        {
            //
            // Read the received character.
            //
            *g_pucData++ = SoftI2CDataGet(&g_sI2C);
            g_ulCount--;

            //
            // Finish the burst read.
            //
            SoftI2CControl(&g_sI2C, SOFTI2C_CMD_BURST_RECEIVE_FINISH);

            //
            // The next state is the wait for final read state.
            //
            g_ulState = STATE_READ_WAIT;

            //
            // This state is done.
            //
            break;
        }

        //
        // This state is for the final read of a single or burst read.
        //
        case STATE_READ_WAIT:
        {
            //
            // Read the received character.
            //
            *g_pucData++ = SoftI2CDataGet(&g_sI2C);
            g_ulCount--;

            //
            // The state machine is now idle.
            //
            g_ulState = STATE_IDLE;

            //
            // This state is done.
            //
            break;
        }
    }
}

void read(unsigned char *pucData, unsigned long ulOffset, unsigned long ulCount)
{
    //
    // Save the data buffer to be read.
    //
    g_pucData = pucData;
    g_ulCount = ulCount;

    //
    // Set the next state of the callback state machine based on the number of
    // bytes to read.
    //
    if(ulCount == 1)
    {
        g_ulState = STATE_READ_ONE;
    }
    else
    {
        g_ulState = STATE_READ_FIRST;
    }

    //
    // Start with a dummy write to get the address set in the EEPROM.
    //
    SoftI2CSlaveAddrSet(&g_sI2C, SLAVE_ADDR | (ulOffset >> 8), false);

    //
    // Write the address to be written as the first data byte.
    //
    SoftI2CDataPut(&g_sI2C, ulOffset);

    //
    // Perform a single send, writing the address as the only byte.
    //
    SoftI2CControl(&g_sI2C, SOFTI2C_CMD_SINGLE_SEND);

    //
    // Wait until the SoftI2C callback state machine is idle.
    //
    while(g_ulState != STATE_IDLE)
    {
    }
}

//*****************************************************************************
//
// This is the interrupt handler for the Timer0A interrupt.
//
//*****************************************************************************
void
Timer0AIntHandler(void)
{
    //
    // Clear the timer interrupt.
    // TODO: change this to whichever timer you are using.
    //
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Call the SoftI2C tick function.
    //
    SoftI2CTimerTick(&g_sI2C);
}

void write(unsigned char *pucData, unsigned long ulOffset,
           unsigned long ulCount)
{
    //
    // Save the data buffer to be written.
    //
    g_pucData = pucData;
    g_ulCount = ulCount;

    //
    // Set the next state of the callback state machine based on the number of
    // bytes to write.
    //
    if(ulCount != 1)
    {
        g_ulState = STATE_WRITE_NEXT;
    }
    else
    {
        g_ulState = STATE_WRITE_FINAL;
    }

    //
    // Set the slave address and setup for a transmit operation.
    //
    SoftI2CSlaveAddrSet(&g_sI2C, SLAVE_ADDR | (ulOffset >> 8), false);

    //
    // Write the address to be written as the first data byte.
    //
    SoftI2CDataPut(&g_sI2C, ulOffset);

    //
    // Start the burst cycle, writing the address as the first byte.
    //
    SoftI2CControl(&g_sI2C, SOFTI2C_CMD_BURST_SEND_START);

    //
    // Wait until the SoftI2C callback state machine is idle.
    //
    while(g_ulState != STATE_IDLE)
    {
    }
}

int main(void){
	//todo: make slave address global variable, as we'll have multiple of them and it should be set
	//in each read or write operation
	int * debug;
	//necessary variable definitions
	const int FORWARD_CHARACTER = 0;
	const int REVERSE_CHARACTER = 1;
	int last = 0;
	int count = 0;

	volatile int state = 0; //unsure if this will work


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
