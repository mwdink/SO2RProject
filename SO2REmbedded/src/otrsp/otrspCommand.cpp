/*
 * otrspCommand.cpp
 *
 *  Created on: Jul 28, 2011
 *      Author: dinkem1
 */

#include "otrsp.hpp"
#include <string.h>
#include <stdio.h>

extern char responseBuffer[];
char nullString[] = "";


/*!Function     Otrsp::parseCommand
*   \param      char *instructionString
*   \return     otrspEnumeration
*   \par Purpose
*
*/
otrspEnumeration
Otrsp::parseCommand(char *commandString)
{
    otrspEnumeration command = UNKNOWN;

    switch(commandString[0])
    {
    case 'A':
    case 'B':
    case 'E':
    case 'F':
    case 'M':
        command = UNSUPPORTED;
        break;
    case 'N':
        command = NAME;
        break;
    case 'R':
        command = RX;
        break;
    case 'T':
        command = TX;
        break;
    case 'C':
        command = CR0;
        break;
    default:
        break;
    }
    return command;
}


/*!Function     Otrsp::respondToCommand
*   \param      otrspEnumeration command
*   \param      char *pCommand
*   \return     char *  (response string, if any)
*   \par Purpose
*
*/
char *
Otrsp::respondToCommand(otrspEnumeration command, char *pCommand)
{
    memset(responseBuffer, 0x00, MAX_RESPONSE);

    switch(command)
    {
    case NAME:
        /* there is no response to the NAME command */
        break;
    case TX:
        strlcpy(responseBuffer, respondToTxCommand(pCommand), MAX_RESPONSE);
        break;
    case RX:
        strlcpy(responseBuffer, respondToRxCommand(pCommand), MAX_RESPONSE);
        break;
    case CR0:
        strlcpy(responseBuffer, respondToPttCommand(pCommand), MAX_RESPONSE);
        break;
    default:
        break;
    }
    return responseBuffer;
}


/*!Function     Otrsp::respondToTxCommand
*   \param      char *pCommand
*   \return     char *  (response string)
*   \par Purpose
*               respond to a TX command
*/
char *
Otrsp::respondToTxCommand(char *pCommand)
{
    char *pResponse = nullString;

    if(MANUAL_SELECT_AUTO == getTxSwitch())
    {
        if('1' == pCommand[2])
        {
            if(MANUAL_SELECT_1 != getTxFocus())
            {
                setTxFocus(MANUAL_SELECT_1);
            }
        }
        else if('2' == pCommand[2])
        {
            if(MANUAL_SELECT_2 != getTxFocus())
            {
                setTxFocus(MANUAL_SELECT_2);
            }
        }
    }
    return pResponse;
}

/*!Function     Otrsp::respondToRxCommand
*   \param      char *pCommand
*   \return     char *  (response string)
*   \par Purpose
*               respond to a RX command
*/
char *
Otrsp::respondToRxCommand(char *pCommand)
{
    char *pResponse = nullString;

    if(MANUAL_SELECT_AUTO == getRxSwitch())
    {
        if('1' == pCommand[2])
        {
            if(MANUAL_SELECT_1 != getRxFocus())
            {
                setRxFocus(MANUAL_SELECT_1);
            }
        }
        else if('2' == pCommand[2])
        {
            if(MANUAL_SELECT_2 != getRxFocus())
            {
                setRxFocus(MANUAL_SELECT_2);
            }
        }
    }
    return pResponse;
}

/*!Function     Otrsp::respondToPttCommand
*   \param      char *pCommand
*   \return     char *  (response string)
*   \par Purpose
*               respond to a CR0 command
*/
char *
Otrsp::respondToPttCommand(char *pCommand)
{
#define PTT_COMMAND_POS 3

    char *pResponse = nullString;

    if(PTT_OFF == getPttSwitch())
    {
        if('1' == pCommand[PTT_COMMAND_POS])
        {
            if(PTT_OFF == getPttState())
            {
                //setPttState(PTT_ON_BY_SWITCH);
                setPttState(PTT_ON_BY_COMMAND);
            }
        }
        else if('0' == pCommand[PTT_COMMAND_POS])
        {
            if(getPttState() != PTT_OFF)
            {
                setPttState(PTT_OFF);
            }
        }
    }
    return pResponse;
}


