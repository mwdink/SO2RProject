/*
 * usartTests.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: mwdink
 */
#include "stdio.h"
#include "usart.hpp"
#include "string.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#include "TestHarness.h"
#include "MockSupport.h"

uint8_t udr = 0;
uint8_t ucsra = 0;
uint8_t ucsrb = 0;
uint8_t ucsrc = 0;
uint8_t ubrrh = 0;
uint8_t ubrrl = 0;


TEST_GROUP(Usart_noSetup)
{
	void setup()
	{;}

	void teardown()
	{;}
};

TEST_GROUP(Usart)
{
	Usart  *pUsart;
	UsartRx *pUsartRx;
    UsartTx *pUsartTx;

	void setup()
	{
		pUsart = new Usart;
		pUsartRx = new UsartRx;
        pUsartTx = new UsartTx;
		pUsart->configureRx(pUsartRx);
        pUsart->configureTx(pUsartTx);
        pUsart->initUsart0(9600);
	}

	void teardown()
	{
	    if(pUsart)
	    {
	    	delete pUsart;
	    }
	    if(pUsartRx)
	    {
	    	delete pUsartRx;
	    }
        if(pUsartTx)
        {
            delete pUsartTx;
        }

	}
};


TEST(Usart_noSetup, usartConstruction)
{
	Usart  *pUsart = 0;
	UsartRx *pUsartRx = 0;

	pUsart = new Usart;
	CHECK(pUsart != 0);
	CHECK(0 == pUsart->getRx());

	pUsartRx = new UsartRx;
	pUsart->configureRx(pUsartRx);
	CHECK(pUsartRx == pUsart->getRx());

	delete pUsartRx;
	delete pUsart;
}


TEST(Usart, usartInit0)
{
    pUsart->initUsart0(9600);

    CHECK(0x02 == USART0_STATUS_REGISTER_A);
    CHECK(0x98 == USART0_STATUS_REGISTER_B);
    CHECK(0x06 == USART0_STATUS_REGISTER_C);
    CHECK(0x00 == USART0_BAUD_RATE_HIGH_BYTE);
    CHECK(0xcf == USART0_BAUD_RATE_LOW_BYTE);
}

TEST(Usart, rxInterrupt)
{
    uint8_t testBuffer[] = "TestString\r";
    uint8_t sizeOfBuffer = strlen((char *)testBuffer);

    for(uint8_t i = 0; i < sizeOfBuffer; ++i)
    {
        pUsart->getRx()->rxInterrupt(testBuffer[i]);
    }
    for(uint8_t i = 0; i < strlen((char *)testBuffer); ++i)
    {
        pUsart->getRx()->rxInterrupt(testBuffer[i]);
    }
    for(uint8_t i = 0; i < (strlen((char *)testBuffer)-1); ++i)
    {
        pUsart->getRx()->rxInterrupt(testBuffer[i]);
    }
    pUsart->getRx()->getIncomingDataBuffer(0);
    CHECK(sizeOfBuffer-1 == pUsart->getRx()->getIncomingDataBuffer(0)->count);
    CHECK(true == pUsart->getRx()->getIncomingDataBuffer(0)->ready);

    pUsart->getRx()->getIncomingDataBuffer(1);
    CHECK(sizeOfBuffer-1 == pUsart->getRx()->getIncomingDataBuffer(1)->count);
    CHECK(true == pUsart->getRx()->getIncomingDataBuffer(1)->ready);

    pUsart->getRx()->getIncomingDataBuffer(2);
    CHECK(sizeOfBuffer-1 == pUsart->getRx()->getIncomingDataBuffer(2)->count);
    CHECK(false == pUsart->getRx()->getIncomingDataBuffer(2)->ready);

    pUsart->getRx()->rxInterrupt('\r');
    pUsart->getRx()->getIncomingDataBuffer(2);
    CHECK(sizeOfBuffer-1 == pUsart->getRx()->getIncomingDataBuffer(2)->count);
    CHECK(true == pUsart->getRx()->getIncomingDataBuffer(2)->ready);

    pUsart->getRx()->getIncomingDataBuffer(0);
    CHECK(0 == pUsart->getRx()->getIncomingDataBuffer(0)->count);
    CHECK(false == pUsart->getRx()->getIncomingDataBuffer(0)->ready);
}

TEST(Usart, checkForIncomingData)
{
    uint8_t testBuffer[] = "TestString\r";

    CHECK(0 == pUsart->getRx()->checkForIncomingData());

    for(uint8_t i = 0; i < strlen((char *)testBuffer); ++i)
    {
        pUsart->getRx()->rxInterrupt(testBuffer[i]);
    }
    CHECK(pUsart->getRx()->getIncomingDataBuffer(0)->buffer ==
                    pUsart->getRx()->checkForIncomingData());

    CHECK(0 == pUsart->getRx()->checkForIncomingData());
}

TEST(Usart, sendOutGoingData0)
{
	ucsra = USART_DATA_REG_IS_EMPTY; //status register needs to be empty

	CHECK(4 == pUsart->getTx()->sendOutGoingData0((char *)"foo"));
	CHECK('\r' == udr);
	STRCMP_EQUAL("foo", pUsart->getTx()->getTxShadowBuffer());

	CHECK(2 == pUsart->getTx()->sendOutGoingData0((char *)"?"));
	CHECK('\r' == udr);
	STRCMP_EQUAL("?", pUsart->getTx()->getTxShadowBuffer());
}



