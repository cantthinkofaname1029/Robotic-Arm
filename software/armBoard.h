/*
 * armBoard.h
 *
 *  Created on: Dec 10, 2014
 *      Author: Drue
 */

#ifndef ARMBOARD_H_
#define ARMBOARD_H_


#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "utils/softi2c.h"
#include "utils/uartstdio.h"

#define DELAY 5
#define BYTE
#define gpioPin 5
#define gpioAddress 5
#define SLAVE_ADDR              0x54

//*****************************************************************************
//
// The states in the interrupt handler state machine.
//
//*****************************************************************************
#define STATE_IDLE              0
#define STATE_WRITE_NEXT        1
#define STATE_WRITE_FINAL       2
#define STATE_WAIT_ACK          3
#define STATE_SEND_ACK          4
#define STATE_READ_ONE          5
#define STATE_READ_FIRST        6
#define STATE_READ_NEXT         7
#define STATE_READ_FINAL        8
#define STATE_READ_WAIT         9







void reset(void);
void InitConsole(void);
void read(unsigned char *pucData, unsigned long ulOffset, unsigned long ulCount);
void Timer0AIntHandler(void);
void write(unsigned char *pucData, unsigned long ulOffset, unsigned long ulCount);
void SoftI2CCallback(void);
void wristClockWise();
void wristCOunterClockWise();
void wristUp();
void wristDown();
void elbowCounterClockWise();
void elbowClockWise();
void elbowDown();
void elbowUp();
void actuatorForward();
void actuatorReverse();
void baseClockWise();
void baseCounterClockWise();
void resetStruct();







#endif /* ARMBOARD_H_ */
