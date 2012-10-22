/*
 * qtrspQuery.cpp
 *
 *  Created on: Jul 28, 2011
 *      Author: dinkem1
 */

#include "otrsp.hpp"
#include <string.h>
#include <stdio.h>

extern char responseBuffer[];

char otrspQueryName[] = "?NAME";
char otrspQueryTX[] = "?TX";
char otrspQueryRX[] = "?RX";
char otrspQueryPTT[] = "?CR0";


char otrspName[] = "SO2R_N7WA";
char otrspPing[] = "?";
char otrspTx[] = "TX0";
char otrspRx[] = "RX0";
char otrspPtt[] = "CR00";



/*!Function     Otrsp::parseCommandForQuery
*   \param      char *commandString
*   \return     bool
*   \par Purpose
*
*/
bool
Otrsp::parseCommandForQuery(char *commandString)
{
    bool isQuery = false;

    if('?' == commandString[0])
    {
        isQuery = true;
    }
    return isQuery;
}

/*!Function     Otrsp::respondToQueryName
*   \return     char *  (response string)
*   \par Purpose
*               Send the response for a NAME Query
*/
char *
Otrsp::respondToQueryName(void)
{
    return otrspName;
}

/*!Function     Otrsp::respondToPing
*   \return     char *  (response string)
*   \par Purpose
*               Send the response for a single ?
*/
char *
Otrsp::respondToPing(void)
{
    return otrspPing;
}

/*!Function     Otrsp::respondToUnsupportedQuery
*   \param      char *pQuery
*   \return     char *  (response string)
*   \par Purpose
*
*/
char *
Otrsp::respondToUnsupportedQuery(char *pQuery)
{
    uint8_t querySize = strlen(pQuery);
    if('\r' == pQuery[querySize-1])
    {
        pQuery[querySize-1] = 0x00;
    }
    return pQuery;
}

/*!Function     Otrsp::respondToQueryTx
*   \param      char *pQuery
*   \return     char *  (response string)
*   \par Purpose
*               Send the response for a single ?TX
*/
char *
Otrsp::respondToQueryTx(char *pQuery)
{
#define TX_POS 2

    uint8_t tx = 0;
    char *pResult = pQuery;

    tx = getTxFocus();
    if(tx)
    {
        pResult = (char *)otrspTx;
        pResult[TX_POS] = '0' + tx;
    }
    return pResult;
}



/*!Function     Otrsp::parseQuery
*   \param      char *queryString
*   \return     otrspEnumeration
*   \par Purpose
*
*/
otrspEnumeration
Otrsp::parseQuery(char *queryString)
{
    otrspEnumeration query = UNKNOWN;

    if(!strcmp(otrspPing, queryString))
    {
        query = RAWQUERY;
    }
    else if(strlen(queryString) > 1)
    {
        switch(queryString[1])
        {
        case 'A':
        case 'B':
        case 'E':
        case 'F':
        case 'M':
            query = UNSUPPORTED;
            break;
        case 'N':
            if(!strcmp(otrspQueryName, queryString))
            {
                query = NAME;
            }
            break;
        case 'R':
            if(!strcmp(otrspQueryRX, queryString))
            {
                query = RX;
            }
            break;
        case 'T':
            if(!strcmp(otrspQueryTX, queryString))
            {
                query = TX;
            }
            break;
        case 'C':
            if(!strcmp(otrspQueryPTT, queryString))
            {
                query = CR0;
            }
            break;
        default:
            break;
        }
    }
    return query;
}

/*!Function     Otrsp::respondToQuery
*   \param      otrspEnumeration query
*   \param      char *pQuery
*   \return     char *  (response string, if any)
*   \par Purpose
*
*/
char *
Otrsp::respondToQuery(otrspEnumeration query, char *pQuery)
{
    memset(responseBuffer, 0x00, MAX_RESPONSE);

    switch(query)
    {
    case TX:
        strlcpy(responseBuffer, respondToQueryTx(pQuery), MAX_RESPONSE);
        break;
    case RX:
        strlcpy(responseBuffer, respondToQueryRx(pQuery), MAX_RESPONSE);
        break;
    case NAME:
        strlcpy(responseBuffer, respondToQueryName(), MAX_RESPONSE);
        break;
    case CR0:
        strlcpy(responseBuffer, respondToQueryPtt(pQuery), MAX_RESPONSE);
        break;
    case RAWQUERY:
        strlcpy(responseBuffer, respondToPing(), MAX_RESPONSE);
        break;
    case UNSUPPORTED:
    case UNKNOWN:
        strlcpy(responseBuffer, respondToUnsupportedQuery(pQuery), MAX_RESPONSE);
        break;
    default:
        break;
    }
    return responseBuffer;
}

/*!Function     Otrsp::respondToQueryTx
*   \param      char *pQuery
*   \return     char *  (response string)
*   \par Purpose
*               Send the response for a single ?RX
*/
char *
Otrsp::respondToQueryRx(char *pQuery)
{
#define RX_POS 2

    uint8_t rx = 0;
    char *pResult = pQuery;

    rx = getRxFocus();
    if(rx)
    {
        pResult = (char *)otrspRx;
        pResult[RX_POS] = '0' + rx;
    }

    return pResult;
}

/*!Function     Otrsp::respondToQueryPtt
*   \param      char *pQuery
*   \return     char *  (response string)
*   \par Purpose
*               Send the response for a ?CR0
*/
char *
Otrsp::respondToQueryPtt(char *pQuery)
{
#define PTT_RESULT_POS 3

    char *pResult = (char *)otrspPtt;
    if(PTT_OFF == getPttState())
    {
        pResult[PTT_RESULT_POS] = '0';
    }
    else
    {
        pResult[PTT_RESULT_POS] = '1';
    }

    return pResult;
}

