/*****************************************************************************/
/* so2rHardware.cpp

!PURPOSE:
    contains hardware specific implementation.
    stuff in here is generally not testable

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
#include <stdint.h>
#include <WProgram.h>
#include "so2rHardware.hpp"


/*!Function     So2rHardware::So2rHardware
*   \par Purpose
*               constructor
*/
So2rHardware::So2rHardware()
{
    configureLedDrivePins();
    configureTxSelectSensePins();
    configureRxSelectSensePins();
    configurePttSelectSensePins();
    setTxFocusHardware(MANUAL_SELECT_1);
    setRxFocusHardware(MANUAL_SELECT_1);
}


/*!Function     So2rHardware::configureLedDrivePins
*   \return     void
*   \par Purpose
*
*/
void
So2rHardware::configureLedDrivePins(void)
{
    pinMode(TX1_LED_PIN, OUTPUT);
    pinMode(TX2_LED_PIN, OUTPUT);
    pinMode(RX1_LED_PIN, OUTPUT);
    pinMode(RX2_LED_PIN, OUTPUT);
    pinMode(PTT_LED_PIN, OUTPUT);
}

/*!Function     So2rHardware::configureTxSelectSensePins
*   \return     void
*   \par Purpose
*
*/
void
So2rHardware::configureTxSelectSensePins(void)
{
    pinMode(SELECT_TX1_PIN, INPUT);
    pinMode(SELECT_TX2_PIN, INPUT);
    /* enable pullup */
    digitalWrite(SELECT_TX1_PIN, HIGH);
    digitalWrite(SELECT_TX2_PIN, HIGH);
}

/*!Function     So2rHardware::configureRxSelectSensePins
*   \return     void
*   \par Purpose
*
*/
void
So2rHardware::configureRxSelectSensePins(void)
{
    pinMode(SELECT_RX1_PIN, INPUT);
    pinMode(SELECT_RX2_PIN, INPUT);
    /* enable pullup */
    digitalWrite(SELECT_RX1_PIN, HIGH);
    digitalWrite(SELECT_RX2_PIN, HIGH);
}

/*!Function     So2rHardware::configurePttSelectSensePins
*   \return     void
*   \par Purpose
*
*/
void
So2rHardware::configurePttSelectSensePins(void)
{
    pinMode(SELECT_PTT_PIN, INPUT);
    /* enable pullup */
    digitalWrite(SELECT_PTT_PIN, HIGH);
}

/*!Function     So2rHardware::getTxSwitch
*   \return     uint8_t
*   \par Purpose
*               determine whether one of the tx select pins
*               is selected
*/
uint8_t
So2rHardware::getTxSwitch(void)
{
    uint8_t selected = MANUAL_SELECT_AUTO;
    int16_t tx1Value = digitalRead(SELECT_TX1_PIN);
    int16_t tx2Value = digitalRead(SELECT_TX2_PIN);

    if(!tx1Value && tx2Value)
    {
        selected = MANUAL_SELECT_1;
    }
    else if(tx1Value && !tx2Value)
    {
        selected = MANUAL_SELECT_2;
    }
    return selected;

}

/*!Function     So2rHardware::getRxSwitch
*   \return     uint8_t
*   \par Purpose
*               determine whether one of the rx select pins
*               is selected
*/
uint8_t
So2rHardware::getRxSwitch(void)
{
    uint8_t selected = MANUAL_SELECT_AUTO;
    int16_t rx1Value = digitalRead(SELECT_RX1_PIN);
    int16_t rx2Value = digitalRead(SELECT_RX2_PIN);

    if(!rx1Value && rx2Value)
    {
        selected = MANUAL_SELECT_1;
    }
    else if(rx1Value && !rx2Value)
    {
        selected = MANUAL_SELECT_2;
    }
    return selected;
}

/*!Function     So2rHardware::getPttSwitch
*   \return     uint8_t
*   \par Purpose
*               determine whether the Ptt pin is selected
*   \note:  PTT is selected when pulled low
*/
uint8_t
So2rHardware::getPttSwitch(void)
{
    uint8_t pttSwitch = PIN_NOT_SELECTED;

    if(PIN_SELECTED == digitalRead(SELECT_PTT_PIN))
    {
        pttSwitch = PIN_SELECTED;
    }

    return pttSwitch;
}

/*!Function     So2rHardware::setTxFocusHardware
*   \param      uint8_t focus
*   \return     void
*   \par Purpose
*               set the hardware to turn on/off the LED and
*               fire/release the relay
*/
void
So2rHardware::setTxFocusHardware(uint8_t focus)
{
	switch(focus)
	{
	case MANUAL_SELECT_1:
	    digitalWrite(TX1_LED_PIN, HIGH);
	    digitalWrite(TX2_LED_PIN, LOW);
		break;
	case MANUAL_SELECT_2:
	    digitalWrite(TX1_LED_PIN, LOW);
	    digitalWrite(TX2_LED_PIN, HIGH);
		break;
	default:
		break;
	}
}

/*!Function     So2rHardware::setRxFocusHardware
*   \param      uint8_t focus
*   \return     void
*   \par Purpose
*               set the hardware to turn on/off the LED and
*               fire/release the relay
*/
void
So2rHardware::setRxFocusHardware(uint8_t focus)
{
	switch(focus)
	{
	case MANUAL_SELECT_1:
	    digitalWrite(RX1_LED_PIN, HIGH);
	    digitalWrite(RX2_LED_PIN, LOW);
		break;
	case MANUAL_SELECT_2:
	    digitalWrite(RX1_LED_PIN, LOW);
	    digitalWrite(RX2_LED_PIN, HIGH);
		break;
	default:
		break;
	};
}

/*!Function     So2rHardware::setPttHardware
*   \param      uint8_t focus
*   \return     void
*   \par Purpose
*               set the hardware to turn on/off the LED and
*               fire/release the relay
*/
void
So2rHardware::setPttHardware(uint8_t select)
{
    if(PTT_OFF == select)
    {
        digitalWrite(PTT_LED_PIN, LOW);
    }
    else
    {
        //PTT is selected or a command was issued by computer
        digitalWrite(PTT_LED_PIN, HIGH);
    }
}
