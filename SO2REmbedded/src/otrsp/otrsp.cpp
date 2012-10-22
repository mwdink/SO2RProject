/*
 * otrsp.cpp
 *
 *  Created on: Jul 8, 2011
 *      Author: dinkem1
 */
/*----------------------------- Nested includes -----------------------------*/
#include "otrsp.hpp"
#include <wiring.h>
#include <string.h>
#include <stdio.h>

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
#define debounceDelay   10      //in milliSec

/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
char responseBuffer[MAX_RESPONSE];


/*!Function     Otrsp::Otrsp
*   \return     none
*   \par Purpose
*               constructor
*/
Otrsp::Otrsp():
txFocus(MANUAL_SELECT_1),
rxFocus(MANUAL_SELECT_1),
pttState(PTT_OFF),
pSo2rHardware(0)
{;}


/*!Function     Otrsp::processOtrspCommunications
*   \return     void
*   \par Purpose
*				process the incoming data
*/
char *
Otrsp::processOtrspCommunications(char *pData)
{
	char *pResponse = 0;

	if(true == parseCommandForQuery(pData))
	{
		pResponse = respondToQuery(parseQuery(pData), pData);
	}
	else
	{
		pResponse = respondToCommand(parseCommand(pData), pData);
	}
	return pResponse;
}


/*!Function     Otrsp::processOtrspHardware
*   \return     void
*   \par Purpose
*               look for hardware input changes
*   \note       only allow check every debounce expiration
*/
void
Otrsp::processOtrspHardware(void)
{
    static uint32_t lastHardwareCheckTime = 0;
    uint32_t currentTime = millis();

    if((currentTime - lastHardwareCheckTime) > debounceDelay)
    {
        checkRxSelectSwitch();
        checkTxSelectSwitch();
        checkForPttSwitchChange();
        lastHardwareCheckTime = currentTime;
    }
}

/*!Function     Otrsp::checkRxSelectSwitch
*   \return     void
*   \par Purpose
*               check for rx focus changes by
*               front panel switch
*/
void
Otrsp::checkRxSelectSwitch(void)
{
    uint8_t rxSwitch;

    rxSwitch = getRxSwitch();

    if(MANUAL_SELECT_AUTO != rxSwitch)
    {
       	if(MANUAL_SELECT_PENDING_DEBOUNCE != rxSwitch)
		{
       		if(getRxFocus() != rxSwitch)
       		{
       			setRxFocus(rxSwitch);
       		}
		}
    }
}

/*!Function     Otrsp::checkTxSelectSwitch
*   \return     void
*   \par Purpose
*               check for tx focus changes by
*               front panel switch
*/
void
Otrsp::checkTxSelectSwitch(void)
{
    uint8_t txSwitch;

    txSwitch = getTxSwitch();

    if(MANUAL_SELECT_AUTO != txSwitch)
    {
    	if(MANUAL_SELECT_PENDING_DEBOUNCE != txSwitch)
    	{
    		if(getTxFocus() != txSwitch)
    		{
    			setTxFocus(txSwitch);
    		}
    	}
    }
}


/*!Function     Otrsp::checkForPttSwitchChange
*   \return     void
*   \par Purpose
*               check for ptt from front panel or foot switch
*/
void
Otrsp::checkForPttSwitchChange(void)
{

    if(getPttState() != PTT_ON_BY_COMMAND)
    {
        uint8_t pttSwitch = getPttSwitch();
        if(pttSwitch != getPttState())
        {
            if(PTT_PENDING_DEBOUNCE != pttSwitch)
            {
                setPttState(pttSwitch);
            }
        }
    }
}



/*!Function     Otrsp::getTxSwitch
*   \return     uint8_t
*   \par Purpose
*               look for hardware input changes
*   \note       debounce is at the otrsp level based on timer
*/
uint8_t
Otrsp::getTxSwitch(void)
{
	static uint8_t lastTxSelect = MANUAL_SELECT_AUTO;
	uint8_t txSelect;

	txSelect = getSo2rHardware()->getTxSwitch();
	if(lastTxSelect != txSelect)
	{
		lastTxSelect = txSelect;
		txSelect = MANUAL_SELECT_PENDING_DEBOUNCE;
	}
    return txSelect;
}

/*!Function     Otrsp::getRxSwitch
*   \return     uint8_t
*               look for hardware input changes
*   \note       debounce is at the otrsp level based on timer
*/
uint8_t
Otrsp::getRxSwitch(void)
{
    static uint8_t lastRxSelect = MANUAL_SELECT_AUTO;
    uint8_t rxSelect;

    rxSelect = getSo2rHardware()->getRxSwitch();
    if(lastRxSelect != rxSelect)
    {
        lastRxSelect = rxSelect;
        rxSelect = MANUAL_SELECT_PENDING_DEBOUNCE;
    }
    return rxSelect;
}

/*!Function     Otrsp::getPttSwitch
*   \return     uint8_t
*   \par Purpose
*               look for hardware input changes
*   \note       debounce is at the otrsp level based on timer
*/
uint8_t
Otrsp::getPttSwitch(void)
{
    static uint8_t lastPttSelect = PTT_OFF;
    uint8_t pttSelect;

    pttSelect = getSo2rHardware()->getPttSwitch();
    if(lastPttSelect != pttSelect)
    {
        /* a change has occurred so wait for debounce period */
        lastPttSelect = pttSelect;
        pttSelect = PTT_PENDING_DEBOUNCE;
    }
    return pttSelect;
}


/*!Function     Otrsp::setTxFocus
*   \param      uint8_t newTxFocus
*   \return     void
*   \par Purpose
*/
void
Otrsp::setTxFocus(uint8_t newTxFocus)
{
	txFocus = newTxFocus;
	getSo2rHardware()->setTxFocusHardware(txFocus);
}

/*!Function     Otrsp::setRxFocus
*   \param      uint8_t newRxFocus
*   \return     void
*   \par Purpose
*/
void
Otrsp::setRxFocus(uint8_t newRxFocus)
{
	rxFocus = newRxFocus;
	getSo2rHardware()->setRxFocusHardware(rxFocus);
}

/*!Function     Otrsp::setPttState
*   \param      uint8_t newState
*   \return     void
*   \par Purpose
*/
void
Otrsp::setPttState(uint8_t newState)
{
    pttState = newState;
    getSo2rHardware()->setPttHardware(pttState);
}
