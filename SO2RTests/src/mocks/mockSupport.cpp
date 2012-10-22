/*
 * mockSupport.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: dinkem1
 */

#include <stdio.h>
#include <string.h>
#include "WProgram.h"
#include "mockHardware.hpp"
#include "mockSupport.hpp"
#include "so2rHardware.hpp"


extern uint8_t portMode[];
extern uint8_t portData[];

/* these are mocks of the hardware input pins */
extern int tx1Pin;
extern int tx2Pin;
extern int rx1Pin;
extern int rx2Pin;
extern int pttPin;
/* mock of the hardware timer */
extern unsigned long milliSeconds;

extern "C"
{
void
pinMode(uint8_t pin, uint8_t mode)
{
    portMode[pin] = mode;
}

void
digitalWrite(uint8_t pin, uint8_t data)
{
    portData[pin] = data;
}

int
digitalRead(uint8_t pin)
{
    int result = 0;
    switch(pin)
    {
    case SELECT_TX1_PIN:
        result = tx1Pin;
        break;
    case SELECT_TX2_PIN:
        result = tx2Pin;
        break;
    case SELECT_RX1_PIN:
        result = rx1Pin;
        break;
    case SELECT_RX2_PIN:
        result = rx2Pin;
        break;
    case SELECT_PTT_PIN:
        result = pttPin;
        break;
    default:
        break;
    }
    return result;
}

void
clearPortArrays(void)
{
    memset(portMode, 0x00, MAX_PIN_ARRAY);
    memset(portData, 0x00, MAX_PIN_ARRAY);
}


void
setMockMillis(uint32_t newMillis)
{
    milliSeconds = newMillis;
}

unsigned long
millis(void)
{
    return milliSeconds;
}


} //extern c

