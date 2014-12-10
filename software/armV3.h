/*
 * armBoardV1.h
 *
 *  Created on: Dec 10, 2014
 *      Author: Drue
 */

#ifndef ARMBOARDV1_H_
#define ARMBOARDV1_H_


#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/softi2c.h"
#include "utils/uartstdio.h"
#include "utils/softuart.h"

#define DELAY 5
#define BYTE



void SoftUARTBreakCtl (tSoftUART *pUART, tBoolean bBreakState);
tBoolean SoftUARTBusy (tSoftUART *pUART);
void SoftUARTCallbackSet (tSoftUART *pUART, void (*pfnCallback)(void));
long SoftUARTCharGet (tSoftUART *pUART);
long SoftUARTCharGetNonBlocking (tSoftUART *pUART);
void SoftUARTCharPut (tSoftUART *pUART, unsigned char ucData);
tBoolean SoftUARTCharPutNonBlocking (tSoftUART *pUART, unsigned char ucData);
tBoolean SoftUARTCharsAvail (tSoftUART *pUART);
void SoftUARTConfigGet (tSoftUART *pUART, unsigned long *pulConfig);
void SoftUARTConfigSet (tSoftUART *pUART, unsigned long ulConfig);
void SoftUARTDisable (tSoftUART *pUART);
void SoftUARTEnable (tSoftUART *pUART);
void SoftUARTFIFOLevelGet (tSoftUART *pUART, unsigned long *pulTxLevel, unsigned long *pulRxLevel);
void SoftUARTFIFOLevelSet (tSoftUART *pUART, unsigned long ulTxLevel, unsigned long ulRxLevel);
void SoftUARTInit (tSoftUART *pUART);
void SoftUARTIntClear (tSoftUART *pUART, unsigned long ulIntFlags);
void SoftUARTIntDisable (tSoftUART *pUART, unsigned long ulIntFlags);
void SoftUARTIntEnable (tSoftUART *pUART, unsigned long ulIntFlags);
unsigned long SoftUARTIntStatus (tSoftUART *pUART, tBoolean bMasked);
unsigned long SoftUARTParityModeGet (tSoftUART *pUART);
void SoftUARTParityModeSet (tSoftUART *pUART, unsigned long ulParity);
void SoftUARTRxBufferSet (tSoftUART *pUART, unsigned short *pusRxBuffer, unsigned short usLen);
void SoftUARTRxErrorClear (tSoftUART *pUART);
unsigned long SoftUARTRxErrorGet (tSoftUART *pUART);
void SoftUARTRxGPIOSet (tSoftUART *pUART, unsigned long ulBase, unsigned char ucPin);
unsigned long SoftUARTRxTick (tSoftUART *pUART, tBoolean bEdgeInt);
tBoolean SoftUARTSpaceAvail (tSoftUART *pUART);
void SoftUARTTxBufferSet (tSoftUART *pUART, unsigned char *pucTxBuffer, unsigned short usLen);
void SoftUARTTxGPIOSet (tSoftUART *pUART, unsigned long ulBase, unsigned char ucPin);
void SoftUARTTxTimerTick (tSoftUART *pUART);


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
