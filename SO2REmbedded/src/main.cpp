/*****************************************************************************/
/* main.cpp

!PURPOSE:
    contains all the hardware specific setups, initializations, and
    the primary task loop. stuff in here is generally not testable

!SUBSYSTEM:

!DEPENDENCIES, LIMITATIONS & DESIGN NOTES:

!REVISION:
    $File:  $
    $Change:  $
    $Revision:  $
    $DateTime:  $
    $Author:    $

Copyright (c) 2011 - All rights reserved.
***************************************************************************/
/*----------------------------- Nested includes -----------------------------*/
#include <wiring.h>
#include <WProgram.h>
#include <HardwareSerial.h>

#include "stdioFixup.hpp"
#include "usart.hpp"
#include "otrsp.hpp"
#include "so2rHardware.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
#define BAUD    9600

/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
Usart  *pUsart0 = 0;
UsartRx *pUsartRx0 = 0;
UsartTx *pUsartTx0 = 0;
Otrsp *pOtrsp = 0;
Usart  *pUsart1 = 0;


/*------------------------------ Declarations -------------------------------*/
void checkForIncomingData(void);
void monitorHardware(void);


int main(void)
{
    init();
    setup();

    for (;;)
    {
        loop();
    }
    return 0;
}


/*!Function     setup
*   \return     void
*   \par Purpose
*               perform one-time initializations, attach hardware
*               implementations
*/
void setup(void)
{
    pUsart0 = new Usart;
    pUsart0->initUsart0(BAUD);
    pUsart0->configureRx(new UsartRx);
    pUsart0->configureTx(new UsartTx);

    pOtrsp = new Otrsp;
    pOtrsp->configureSo2rHardware(new So2rHardware);

    /* this one is just being used for debug right now */
    pUsart1 = new Usart;
    pUsart1->initUsart1(BAUD);

    setupStdIo();

    /* enable interrupts */
    sei();

    pinMode(13, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(6, OUTPUT);
}



/*!Function     loop
*   \return     void
*   \par Purpose
*               main task loop for application
*/
void loop(void)
{
	checkForIncomingData();
    monitorHardware();


}


/*!Function     checkForIncomingData
*   \return     void
*   \par Purpose
*
*/
void checkForIncomingData(void)
{
    char *pResponse = 0;

    uint8_t *pData = pUsart0->getRx()->checkForIncomingData();
    if(pData)
    {
        fprintf(stderr, "%s\n", (char *)pData);
        pResponse = pOtrsp->processOtrspCommunications((char *)pData);
        if(pResponse)
        {
            if(strlen(pResponse))
            {
                pUsart0->getTx()->sendOutGoingData0(pResponse);
            }
        }
    }
}


/*!Function     monitorHardware
*   \return     void
*   \par Purpose
*
*/
void monitorHardware(void)
{
    pOtrsp->processOtrspHardware();
}



/*!Function     ISR(USART0_RX_vect)
*   \return     none
*   \par Purpose
*               Interrupt Service Routine for the RX0
*/
ISR(USART0_RX_vect)
{
    pUsart0->getRx()->rxInterrupt(USART0_TX_RX_REGISTER);
}



