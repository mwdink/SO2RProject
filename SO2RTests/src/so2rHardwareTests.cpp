/*
 * so2rHardwareTests.cpp
 *
 *  Created on: Jul 18, 2011
 *      Author: dinkem1
*/
#include <stdio.h>
#include <string.h>
#include "WProgram.h"
#include "mocks/mockHardware.hpp"
#include "mocks/mockSupport.hpp"
#include "so2rHardware.hpp"

#include "TestHarness.h"
#include "MockSupport.h"


#define LED_ON                  1
#define LED_OFF                 0

uint8_t portMode[MAX_PIN_ARRAY];
uint8_t portData[MAX_PIN_ARRAY];

/* these are mocks of the hardware input pins */
int tx1Pin = 1;	/* active low */
int tx2Pin = 1;
int rx1Pin = 1;
int rx2Pin = 1;
int pttPin = 1;

/* mock of the hardware timer */
unsigned long milliSeconds = 0;




class testableSo2rHardware : public So2rHardware
{
public:
    testableSo2rHardware() : So2rHardware() {;}
    virtual ~testableSo2rHardware(){;}
};

TEST_GROUP(so2rHardware_noSetup)
{
    void setup(){;}
    void teardown(){;}
};


TEST_GROUP(so2rHardware_Setup)
{
    testableSo2rHardware *pSo2rHardware;
    void setup()
    {
        pSo2rHardware = new testableSo2rHardware;
        clearPortArrays();
    }

    void teardown()
    {
        if(pSo2rHardware)
        {
            delete pSo2rHardware;
        }
    }
};

TEST(so2rHardware_noSetup, construction)
{
    testableSo2rHardware *pSo2rHardware = 0;

    pSo2rHardware = new testableSo2rHardware;
    CHECK(pSo2rHardware != 0);

    /* verify output pin settings */
    CHECK(OUTPUT == portMode[TX1_LED_PIN]);
    CHECK(OUTPUT == portMode[TX2_LED_PIN]);
    CHECK(OUTPUT == portMode[RX1_LED_PIN]);
    CHECK(OUTPUT == portMode[RX2_LED_PIN]);
    CHECK(OUTPUT == portMode[PTT_LED_PIN]);
    CHECK(INPUT == portMode[SELECT_TX1_PIN]);
    CHECK(INPUT == portMode[SELECT_TX2_PIN]);
    CHECK(INPUT == portMode[SELECT_RX1_PIN]);
    CHECK(INPUT == portMode[SELECT_RX2_PIN]);
    CHECK(INPUT == portMode[SELECT_PTT_PIN]);

    delete pSo2rHardware;
}

TEST(so2rHardware_Setup, getTxSwitch)
{
    tx1Pin = PIN_NOT_SELECTED;
    tx2Pin = PIN_NOT_SELECTED;
    CHECK(MANUAL_SELECT_AUTO == pSo2rHardware->getTxSwitch());
    tx1Pin = PIN_SELECTED;
    CHECK(MANUAL_SELECT_1 == pSo2rHardware->getTxSwitch());
    tx1Pin = PIN_NOT_SELECTED;
    tx2Pin = PIN_SELECTED;
    CHECK(MANUAL_SELECT_2 == pSo2rHardware->getTxSwitch());
    tx2Pin = PIN_NOT_SELECTED;
    CHECK(MANUAL_SELECT_AUTO == pSo2rHardware->getTxSwitch());
}

TEST(so2rHardware_Setup, getRxSwitch)
{
    rx1Pin = PIN_NOT_SELECTED;
    rx2Pin = PIN_NOT_SELECTED;
    CHECK(MANUAL_SELECT_AUTO == pSo2rHardware->getRxSwitch());
    rx1Pin = PIN_SELECTED;
    CHECK(MANUAL_SELECT_1 == pSo2rHardware->getRxSwitch());
    rx1Pin = PIN_NOT_SELECTED;
    rx2Pin = PIN_SELECTED;
    CHECK(MANUAL_SELECT_2 == pSo2rHardware->getRxSwitch());
    rx2Pin = PIN_NOT_SELECTED;
    CHECK(MANUAL_SELECT_AUTO == pSo2rHardware->getRxSwitch());
}

TEST(so2rHardware_Setup, getPttSwitch)
{
    pttPin = PIN_SELECTED;
    CHECK(PTT_ON_BY_SWITCH == pSo2rHardware->getPttSwitch());
    pttPin = PIN_NOT_SELECTED;
    CHECK(PTT_OFF == pSo2rHardware->getPttSwitch());
}

TEST(so2rHardware_Setup, setTxFocusHardware)
{
	pSo2rHardware->setTxFocusHardware(MANUAL_SELECT_1);
    CHECK(LED_ON == portData[TX1_LED_PIN]);
    CHECK(LED_OFF == portData[TX2_LED_PIN]);
	pSo2rHardware->setTxFocusHardware(MANUAL_SELECT_2);
    CHECK(LED_OFF == portData[TX1_LED_PIN]);
    CHECK(LED_ON == portData[TX2_LED_PIN]);
}

TEST(so2rHardware_Setup, setRxFocusHardware)
{
	pSo2rHardware->setRxFocusHardware(MANUAL_SELECT_1);
    CHECK(LED_ON == portData[RX1_LED_PIN]);
    CHECK(LED_OFF == portData[RX2_LED_PIN]);
	pSo2rHardware->setRxFocusHardware(MANUAL_SELECT_2);
    CHECK(LED_OFF == portData[RX1_LED_PIN]);
    CHECK(LED_ON == portData[RX2_LED_PIN]);
}


