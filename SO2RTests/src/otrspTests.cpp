/*
 * otrspTests.cpp
 *
 *  Created on: Jul 8, 2011
 *      Author: dinkem1
 */

#include <stdio.h>
#include <string.h>
#include "otrsp.hpp"
#include "mocks/mockHardware.hpp"
#include "mocks/mockSupport.hpp"
#include "TestHarness.h"
#include "MockSupport.h"


extern char otrspName[];

char otrspQueryNameResponse[] = "SO2R_N7WA";
char otrspQueryTXResponse1[] = "TX1";
char otrspQueryTXResponse2[] = "TX2";
char otrspQueryRXResponse1[] = "RX1";
char otrspQueryRXResponse2[] = "RX2";
char otrspQueryPttResponseOff[] = "CR00";
char otrspQueryPttResponseOn[] = "CR01";

char queryTx[] = "?TX";
char queryRx[] = "?RX";
char queryName[] = "?NAME";
char queryPtt[] = "?CR0";
char rawQuery[] = "?";
char commandName[] = "NAME\r";
char commandTx1[] = "TX1\r";
char commandTx2[] = "TX2\r";
char commandRx1[] = "RX1\r";
char commandRx2[] = "RX2\r";
char commandPttOff[] = "CR00";
char commandPttOn[] = "CR01";



class testableOtrsp : public Otrsp
{
public:
	testableOtrsp(){;}
    virtual ~testableOtrsp(){;}

    bool accessProtectedParseCommandForQuery(char *commandString){return parseCommandForQuery(commandString);}
    otrspEnumeration accessProtectedParseQuery(char *queryString){return parseQuery(queryString);}
    otrspEnumeration accessProtectedParseCommand(char *instructionString){return parseCommand(instructionString);}
    char *accessProtectedRespondToQuery(otrspEnumeration query, char *pQuery){return respondToQuery(query, pQuery);}
    char *accessProtectedRespondToCommand(otrspEnumeration command, char *pCommand){return respondToCommand(command, pCommand);}
    char *accessProtectedRespondToQueryName(void){return respondToQueryName();}
    char *accessProtectedRespondToPing(void){return respondToPing();}
    char *accessProtectedRespondToQueryTx(char *pQuery){return respondToQueryTx(pQuery);}
    char *accessprotectedRespondToQueryRx(char *pQuery){return respondToQueryRx(pQuery);}
    char *accessprotectedRespondToQueryPtt(char *pQuery){return respondToQueryPtt(pQuery);}
    char *accessProtectedRespondToTxCommand(char *pCommand){return respondToTxCommand(pCommand);}
    char *accessProtectedRespondToRxCommand(char *pCommand){return respondToRxCommand(pCommand);}
    char *accessProtectedRespondToPttCommand(char *pCommand){return respondToPttCommand(pCommand);}
    uint8_t accessProtectedGetRxSwitch(void){return getRxSwitch();}
    uint8_t accessProtectedGetTxSwitch(void){return getTxSwitch();}
    uint8_t accessProtectedGetPttSwitch(void){return getPttSwitch();}
    void accessProtectedSetRxFocus(uint8_t newFocus){setRxFocus(newFocus);}
    void accessProtectedSetTxFocus(uint8_t newFocus){setTxFocus(newFocus);}
    void accessProtectedSetPttState(uint8_t newState){setPttState(newState);}
    uint8_t accessProtectedGetRxFocus(void){return getRxFocus();}
    uint8_t accessProtectedGetTxFocus(void){return getTxFocus();}
    uint8_t accessProtectedGetPttState(void){return getPttState();}
};


class mockCheckSelectOtrsp : public Otrsp
{
public:
    mockCheckSelectOtrsp(){;}
    virtual ~mockCheckSelectOtrsp(){;}

    void    checkRxSelectSwitch(void){mock().actualCall("checkRxSelectSwitch");}
    void    checkTxSelectSwitch(void){mock().actualCall("checkTxSelectSwitch");}
    void    checkForPttSwitchChange(void){mock().actualCall("checkForPttSwitchChange");}
};

class mockGetSelectOtrsp : public Otrsp
{
public:
    mockGetSelectOtrsp(){;}
    virtual ~mockGetSelectOtrsp(){;}

    uint8_t getRxSwitch(void){return mock().actualCall("getRxSwitch").returnValue().getIntValue();}
    uint8_t getTxSwitch(void){return mock().actualCall("getTxSwitch").returnValue().getIntValue();}
    uint8_t getPttSwitch(void){return mock().actualCall("getPttSwitch").returnValue().getIntValue();}
    void accessProtectedCheckRxSelectSwitch(void){checkRxSelectSwitch();}
    void accessProtectedCheckTxSelectSwitch(void){checkTxSelectSwitch();}
    void accessProtectedCheckForPttSwitchChange(void){checkForPttSwitchChange();}
    void accessProtectedSetRxFocus(uint8_t newFocus){setRxFocus(newFocus);}
    void accessProtectedSetTxFocus(uint8_t newFocus){setTxFocus(newFocus);}
    void accessProtectedSetPttState(uint8_t newState){setPttState(newState);}
    uint8_t accessProtectedGetRxFocus(void){return getRxFocus();}
    uint8_t accessProtectedGetTxFocus(void){return getTxFocus();}
    uint8_t accessProtectedGetPttState(void){return getPttState();}

};

TEST_GROUP(Otrsp_noSetup)
{
    void setup()
    {;}

    void teardown()
    {;}
};


TEST_GROUP(TestableOtrsp_Setup)
{
    testableOtrsp *pOtrsp;
	MockHardware *pSo2rHardware;

    void setup()
    {
        pOtrsp = new testableOtrsp;
        pSo2rHardware = new MockHardware;
        pOtrsp->configureSo2rHardware(pSo2rHardware);
    }

    void teardown()
    {
        /* this is a cleanup so following tests start at a known place */
        pSo2rHardware->setMockRxSelect(MANUAL_SELECT_AUTO);  /* switch is neutral */
        pOtrsp->accessProtectedGetRxSwitch();
        pSo2rHardware->setMockTxSelect(MANUAL_SELECT_AUTO);  /* switch is neutral */
        pOtrsp->accessProtectedGetTxSwitch();
        pSo2rHardware->setMockPttSwitch(PTT_OFF);
        pOtrsp->accessProtectedGetPttSwitch();

        if(pSo2rHardware)
        {
            delete pSo2rHardware;
        }
        if(pOtrsp)
        {
            delete pOtrsp;
        }
    }
};



TEST(Otrsp_noSetup, otrspConstruction)
{
    Otrsp *pOtrsp = 0;
    So2rHardware *pSo2rHardware = 0;

    pOtrsp = new Otrsp;
    pSo2rHardware = new So2rHardware;

    CHECK(pOtrsp != 0);
    CHECK(0 == pOtrsp->getSo2rHardware());
    pOtrsp->configureSo2rHardware(pSo2rHardware);
    CHECK(pSo2rHardware == pOtrsp->getSo2rHardware());

    delete pSo2rHardware;
    delete pOtrsp;
}

TEST(TestableOtrsp_Setup, parseCommandForQuery)
{
    CHECK(true == pOtrsp->accessProtectedParseCommandForQuery((char *)"?"));
    CHECK(false == pOtrsp->accessProtectedParseCommandForQuery((char *)""));
    CHECK(true == pOtrsp->accessProtectedParseCommandForQuery((char *)"? TX"));
    CHECK(true == pOtrsp->accessProtectedParseCommandForQuery((char *)"?TX"));
    CHECK(false == pOtrsp->accessProtectedParseCommandForQuery((char *)"TX"));
}

TEST(TestableOtrsp_Setup, parseQuery)
{
    CHECK(UNSUPPORTED == pOtrsp->accessProtectedParseQuery((char *)"?Axxx"));
    CHECK(UNSUPPORTED == pOtrsp->accessProtectedParseQuery((char *)"?Bxx"));
    CHECK(CR0 == pOtrsp->accessProtectedParseQuery((char *)"?CR0"));
    CHECK(UNKNOWN == pOtrsp->accessProtectedParseQuery((char *)"?CR1"));
    CHECK(UNSUPPORTED == pOtrsp->accessProtectedParseQuery((char *)"?Exxx"));
    CHECK(UNSUPPORTED == pOtrsp->accessProtectedParseQuery((char *)"?Mxx"));
    CHECK(UNKNOWN == pOtrsp->accessProtectedParseQuery((char *)"?NaME"));
    CHECK(NAME == pOtrsp->accessProtectedParseQuery((char *)"?NAME"));
    CHECK(RX == pOtrsp->accessProtectedParseQuery((char *)"?RX"));
    CHECK(TX == pOtrsp->accessProtectedParseQuery((char *)"?TX"));
    CHECK(UNKNOWN == pOtrsp->accessProtectedParseQuery((char *)"?ZZZZZ"));
    CHECK(UNKNOWN == pOtrsp->accessProtectedParseQuery((char *)"?S"));
    CHECK(RAWQUERY == pOtrsp->accessProtectedParseQuery((char *)"?"));
}

TEST(TestableOtrsp_Setup, parseCommand)
{
    CHECK(UNSUPPORTED == pOtrsp->accessProtectedParseCommand((char *)"Axxx"));
    CHECK(UNSUPPORTED == pOtrsp->accessProtectedParseCommand((char *)"Bxxx"));
    CHECK(CR0 == pOtrsp->accessProtectedParseCommand((char *)"Cxxx"));
    CHECK(UNSUPPORTED == pOtrsp->accessProtectedParseCommand((char *)"Exxx"));
    CHECK(UNSUPPORTED == pOtrsp->accessProtectedParseCommand((char *)"Mxxx"));
    CHECK(NAME == pOtrsp->accessProtectedParseCommand((char *)"Nxxx"));
    CHECK(RX == pOtrsp->accessProtectedParseCommand((char *)"Rzzz"));
    CHECK(TX == pOtrsp->accessProtectedParseCommand((char *)"Tzzz"));
    CHECK(UNKNOWN == pOtrsp->accessProtectedParseCommand((char *)"Z"));
    CHECK(UNKNOWN == pOtrsp->accessProtectedParseCommand((char *)"S"));
}

TEST(TestableOtrsp_Setup, respondToQueryName)
{
	STRCMP_EQUAL(otrspQueryNameResponse, pOtrsp->accessProtectedRespondToQueryName());
}

TEST(TestableOtrsp_Setup, respondToPing)
{
    STRCMP_EQUAL("?", pOtrsp->accessProtectedRespondToPing());
}

TEST(TestableOtrsp_Setup, respondToQueryTx)
{
    STRCMP_EQUAL(otrspQueryTXResponse1, pOtrsp->accessProtectedRespondToQueryTx(queryTx));
    pOtrsp->accessProtectedSetTxFocus(FOCUS_2);
    STRCMP_EQUAL(otrspQueryTXResponse2, pOtrsp->accessProtectedRespondToQueryTx(queryTx));
    pOtrsp->accessProtectedSetTxFocus(FOCUS_1);
    STRCMP_EQUAL(otrspQueryTXResponse1, pOtrsp->accessProtectedRespondToQueryTx(queryTx));
}

TEST(TestableOtrsp_Setup, respondToQueryRx)
{
    STRCMP_EQUAL(otrspQueryRXResponse1, pOtrsp->accessprotectedRespondToQueryRx(queryRx));
    pOtrsp->accessProtectedSetRxFocus(FOCUS_2);
    STRCMP_EQUAL(otrspQueryRXResponse2, pOtrsp->accessprotectedRespondToQueryRx(queryRx));
    pOtrsp->accessProtectedSetRxFocus(FOCUS_1);
    STRCMP_EQUAL(otrspQueryRXResponse1, pOtrsp->accessprotectedRespondToQueryRx(queryRx));
}

TEST(TestableOtrsp_Setup, respondToQueryPtt)
{
    STRCMP_EQUAL(otrspQueryPttResponseOff, pOtrsp->accessprotectedRespondToQueryPtt(queryPtt));
    pOtrsp->accessProtectedSetPttState(PTT_ON_BY_SWITCH);
    STRCMP_EQUAL(otrspQueryPttResponseOn, pOtrsp->accessprotectedRespondToQueryPtt(queryPtt));
    pOtrsp->accessProtectedSetPttState(PTT_OFF);
    STRCMP_EQUAL(otrspQueryPttResponseOff, pOtrsp->accessprotectedRespondToQueryPtt(queryPtt));
}



TEST(TestableOtrsp_Setup, respondToQuery)
{
    char source[10] = "";

    strcpy(source, queryName);
	STRCMP_EQUAL(otrspQueryNameResponse, pOtrsp->accessProtectedRespondToQuery(NAME, source));
    strcpy(source, rawQuery);
    STRCMP_EQUAL("?", pOtrsp->accessProtectedRespondToQuery(RAWQUERY, source));

    strcpy(source, queryTx);
    STRCMP_EQUAL(otrspQueryTXResponse1, pOtrsp->accessProtectedRespondToQuery(TX, source));
    strcpy(source, queryRx);
    STRCMP_EQUAL(otrspQueryRXResponse1, pOtrsp->accessProtectedRespondToQuery(RX, source));

    pOtrsp->accessProtectedSetRxFocus(FOCUS_1);
    pOtrsp->accessProtectedSetTxFocus(FOCUS_2);
    strcpy(source, queryTx);
    STRCMP_EQUAL(otrspQueryTXResponse2, pOtrsp->accessProtectedRespondToQuery(TX, source));
    strcpy(source, queryRx);
    STRCMP_EQUAL(otrspQueryRXResponse1, pOtrsp->accessProtectedRespondToQuery(RX, source));

    pOtrsp->accessProtectedSetRxFocus(FOCUS_2);
    pOtrsp->accessProtectedSetTxFocus(FOCUS_1);
    strcpy(source, queryTx);
    STRCMP_EQUAL(otrspQueryTXResponse1, pOtrsp->accessProtectedRespondToQuery(TX, source));
    strcpy(source, queryRx);
    STRCMP_EQUAL(otrspQueryRXResponse2, pOtrsp->accessProtectedRespondToQuery(RX, source));
}

TEST(TestableOtrsp_Setup, respondToCommand)
{
    char source[10] = "";

    strcpy(source, commandName);
    STRCMP_EQUAL("", pOtrsp->accessProtectedRespondToCommand(NAME, source));
    strcpy(source, commandTx1);
    STRCMP_EQUAL("", pOtrsp->accessProtectedRespondToCommand(TX, source));
    strcpy(source, commandRx2);
    STRCMP_EQUAL("", pOtrsp->accessProtectedRespondToCommand(RX, source));
}


TEST(TestableOtrsp_Setup, processOtrspCommunications)
{
	STRCMP_EQUAL(otrspQueryNameResponse, pOtrsp->processOtrspCommunications((char *)"?NAME"));
    STRCMP_EQUAL("?", pOtrsp->processOtrspCommunications((char *)"?"));
    STRCMP_EQUAL("?AUDIO", pOtrsp->processOtrspCommunications((char *)"?AUDIO"));
    STRCMP_EQUAL("?AUDIO", pOtrsp->processOtrspCommunications((char *)"?AUDIO"));
    STRCMP_EQUAL("?AUX0", pOtrsp->processOtrspCommunications((char *)"?AUX0"));
}

TEST(TestableOtrsp_Setup, processOtrsp_RxTx)
{
    char source[10] = "";

    strcpy(source, queryTx);
    STRCMP_EQUAL(otrspQueryTXResponse1, pOtrsp->processOtrspCommunications(source));
    strcpy(source, queryRx);
    STRCMP_EQUAL(otrspQueryRXResponse1, pOtrsp->processOtrspCommunications(source));

    pOtrsp->accessProtectedSetTxFocus(FOCUS_2);
    pOtrsp->accessProtectedSetRxFocus(FOCUS_1);
    strcpy(source, queryTx);
    STRCMP_EQUAL(otrspQueryTXResponse2, pOtrsp->processOtrspCommunications(source));
    strcpy(source, queryRx);
    STRCMP_EQUAL(otrspQueryRXResponse1, pOtrsp->processOtrspCommunications(source));

    pOtrsp->accessProtectedSetTxFocus(FOCUS_1);
    pOtrsp->accessProtectedSetRxFocus(FOCUS_2);
    strcpy(source, queryTx);
    STRCMP_EQUAL(otrspQueryTXResponse1, pOtrsp->processOtrspCommunications(source));
    strcpy(source, queryRx);
    STRCMP_EQUAL(otrspQueryRXResponse2, pOtrsp->processOtrspCommunications(source));
}

TEST(TestableOtrsp_Setup, txFocus)
{
   CHECK(1 == pOtrsp->accessProtectedGetTxFocus());
   pOtrsp->accessProtectedSetTxFocus(FOCUS_2);
   CHECK(FOCUS_2 == pOtrsp->accessProtectedGetTxFocus());
   pOtrsp->accessProtectedSetTxFocus(FOCUS_1);
   CHECK(FOCUS_1 == pOtrsp->accessProtectedGetTxFocus());
}

TEST(TestableOtrsp_Setup, rxFocus)
{
   CHECK(FOCUS_1 == pOtrsp->accessProtectedGetRxFocus());
   pOtrsp->accessProtectedSetRxFocus(FOCUS_2);
   CHECK(FOCUS_2 == pOtrsp->accessProtectedGetRxFocus());
   pOtrsp->accessProtectedSetRxFocus(FOCUS_1);
   CHECK(FOCUS_1 == pOtrsp->accessProtectedGetRxFocus());
}


TEST(Otrsp_noSetup, checkTxSelectSwitch)
{
    mockGetSelectOtrsp *pOtrsp;
    MockHardware *pSo2rHardware;

    pOtrsp = new mockGetSelectOtrsp;
    pSo2rHardware = new MockHardware;
    pOtrsp->configureSo2rHardware(pSo2rHardware);

    pOtrsp->accessProtectedSetTxFocus(0);

    mock().expectOneCall("getTxSwitch").andReturnValue(MANUAL_SELECT_1);
    pOtrsp->accessProtectedCheckTxSelectSwitch();
    CHECK(FOCUS_1 == pOtrsp->accessProtectedGetTxFocus());
    mock().checkExpectations();

    mock().expectOneCall("getTxSwitch").andReturnValue(MANUAL_SELECT_2);
    pOtrsp->accessProtectedCheckTxSelectSwitch();
    CHECK(FOCUS_2 == pOtrsp->accessProtectedGetTxFocus());
    mock().checkExpectations();

    mock().expectOneCall("getTxSwitch").andReturnValue(MANUAL_SELECT_AUTO);
    pOtrsp->accessProtectedCheckTxSelectSwitch();
    /* will remain at last selection */
    CHECK(FOCUS_2 == pOtrsp->accessProtectedGetTxFocus());

    mock().expectOneCall("getTxSwitch").andReturnValue(MANUAL_SELECT_PENDING_DEBOUNCE);
    pOtrsp->accessProtectedCheckTxSelectSwitch();
    /* will remain at last selection */
    CHECK(FOCUS_2 == pOtrsp->accessProtectedGetTxFocus());

    mock().checkExpectations();
    mock().clear();

    delete pSo2rHardware;
    delete pOtrsp;
}

TEST(Otrsp_noSetup, checkRxSelectSwitch)
{
    mockGetSelectOtrsp *pOtrsp;
    MockHardware *pSo2rHardware;

    pOtrsp = new mockGetSelectOtrsp;
    pSo2rHardware = new MockHardware;
    pOtrsp->configureSo2rHardware(pSo2rHardware);

    pOtrsp->accessProtectedSetRxFocus(0);

    mock().expectOneCall("getRxSwitch").andReturnValue(MANUAL_SELECT_2);
    pOtrsp->accessProtectedCheckRxSelectSwitch();
    CHECK(MANUAL_SELECT_2 == pOtrsp->accessProtectedGetRxFocus());
    mock().checkExpectations();

    mock().expectOneCall("getRxSwitch").andReturnValue(MANUAL_SELECT_1);
    pOtrsp->accessProtectedCheckRxSelectSwitch();
    CHECK(MANUAL_SELECT_1 == pOtrsp->accessProtectedGetRxFocus());
    mock().checkExpectations();

    mock().expectOneCall("getRxSwitch").andReturnValue(MANUAL_SELECT_AUTO);
    pOtrsp->accessProtectedCheckRxSelectSwitch();
    /* will remain at last selection */
    CHECK(MANUAL_SELECT_1 == pOtrsp->accessProtectedGetRxFocus());

    mock().expectOneCall("getRxSwitch").andReturnValue(MANUAL_SELECT_PENDING_DEBOUNCE);
    pOtrsp->accessProtectedCheckRxSelectSwitch();
    /* will remain at last selection */
    CHECK(MANUAL_SELECT_1 == pOtrsp->accessProtectedGetRxFocus());

    mock().checkExpectations();
    mock().clear();

    delete pSo2rHardware;
    delete pOtrsp;
}



TEST(Otrsp_noSetup, processOtrspHardware)
{
    mockCheckSelectOtrsp *pOtrsp;
    MockHardware *pSo2rHardware;

    pOtrsp = new mockCheckSelectOtrsp;
    pSo2rHardware = new MockHardware;
    pOtrsp->configureSo2rHardware(pSo2rHardware);

    mock().expectOneCall("checkRxSelectSwitch");
    mock().expectOneCall("checkTxSelectSwitch");
    mock().expectOneCall("checkForPttSwitchChange");

    setMockMillis(5);
    pOtrsp->processOtrspHardware();
    setMockMillis(11);
    pOtrsp->processOtrspHardware();
    setMockMillis(14);
    pOtrsp->processOtrspHardware();

    mock().checkExpectations();
    mock().clear();

    delete pSo2rHardware;
    delete pOtrsp;
}

TEST(TestableOtrsp_Setup, getRxSwitch)
{
	pSo2rHardware->setMockRxSelect(MANUAL_SELECT_2);
    CHECK(MANUAL_SELECT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetRxSwitch());
    CHECK(MANUAL_SELECT_2 == pOtrsp->accessProtectedGetRxSwitch());

	pSo2rHardware->setMockRxSelect(MANUAL_SELECT_1);
    CHECK(MANUAL_SELECT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetRxSwitch());

    pSo2rHardware->setMockRxSelect(MANUAL_SELECT_2);  /* switch bounce */
    CHECK(MANUAL_SELECT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetRxSwitch());
    pSo2rHardware->setMockRxSelect(MANUAL_SELECT_1);  /* done bouncing */
    CHECK(MANUAL_SELECT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetRxSwitch());
    CHECK(MANUAL_SELECT_1 == pOtrsp->accessProtectedGetRxSwitch());
}

TEST(TestableOtrsp_Setup, getTxSwitch)
{
    pSo2rHardware->setMockTxSelect(MANUAL_SELECT_1);
    CHECK(MANUAL_SELECT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetTxSwitch());
    CHECK(MANUAL_SELECT_1 == pOtrsp->accessProtectedGetTxSwitch());

    pSo2rHardware->setMockTxSelect(MANUAL_SELECT_2);
    CHECK(MANUAL_SELECT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetTxSwitch());
    pSo2rHardware->setMockTxSelect(MANUAL_SELECT_1);  /* switch bounce */
    CHECK(MANUAL_SELECT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetTxSwitch());
    pSo2rHardware->setMockTxSelect(MANUAL_SELECT_2);  /* done bouncing */
    CHECK(MANUAL_SELECT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetTxSwitch());
    CHECK(MANUAL_SELECT_2 == pOtrsp->accessProtectedGetTxSwitch());

    pSo2rHardware->setMockTxSelect(MANUAL_SELECT_AUTO);  /* switch is neutral */
    CHECK(MANUAL_SELECT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetTxSwitch());
    CHECK(MANUAL_SELECT_AUTO == pOtrsp->accessProtectedGetTxSwitch());
}

TEST(TestableOtrsp_Setup, getPttSwitch)
{
    pSo2rHardware->setMockPttSwitch(PTT_ON_BY_SWITCH);
    CHECK(PTT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetPttSwitch());
    CHECK(PTT_ON_BY_SWITCH == pOtrsp->accessProtectedGetPttSwitch());

    pSo2rHardware->setMockPttSwitch(PTT_OFF);
    CHECK(PTT_PENDING_DEBOUNCE == pOtrsp->accessProtectedGetPttSwitch());
    CHECK(PTT_OFF == pOtrsp->accessProtectedGetPttSwitch());
}

TEST(TestableOtrsp_Setup, respondToTxCommand)
{
    pSo2rHardware->setMockTxSelect(MANUAL_SELECT_AUTO);  /* switch is neutral */
    pOtrsp->accessProtectedGetTxSwitch();

    pOtrsp->accessProtectedSetTxFocus(FOCUS_2); /* current focus */
    pOtrsp->accessProtectedRespondToTxCommand(commandTx1); /* incoming command to go to 1 */
    CHECK(FOCUS_1 == pOtrsp->accessProtectedGetTxFocus());

    pOtrsp->accessProtectedRespondToTxCommand(commandTx2); /* incoming command to go to 2 */
    CHECK(FOCUS_2 == pOtrsp->accessProtectedGetTxFocus());

    pOtrsp->accessProtectedSetTxFocus(FOCUS_1); /* current focus */
    pSo2rHardware->setMockTxSelect(MANUAL_SELECT_1);  /* switch is manually on focus 1 */
    pOtrsp->accessProtectedGetTxSwitch();
    pOtrsp->accessProtectedRespondToTxCommand(commandTx2); /* incoming command to go to 2 */
    CHECK(FOCUS_1 == pOtrsp->accessProtectedGetTxFocus()); /* ignore incoming command - switch has precedence */

    pOtrsp->accessProtectedSetTxFocus(FOCUS_2); /* current focus */
    pSo2rHardware->setMockTxSelect(MANUAL_SELECT_2);  /* switch is manually on focus 1 */
    pOtrsp->accessProtectedGetTxSwitch();
    pOtrsp->accessProtectedRespondToTxCommand(commandTx1); /* incoming command to go to 2 */
    CHECK(FOCUS_2 == pOtrsp->accessProtectedGetTxFocus()); /* ignore incoming command - switch has precedence */
}

TEST(TestableOtrsp_Setup, respondToRxCommand)
{
    pSo2rHardware->setMockRxSelect(MANUAL_SELECT_AUTO);  /* switch is neutral */
    pOtrsp->accessProtectedGetRxSwitch();

    pOtrsp->accessProtectedSetRxFocus(FOCUS_2); /* current focus */
    pOtrsp->accessProtectedRespondToRxCommand(commandRx1); /* incoming command to go to 1 */
    CHECK(FOCUS_1 == pOtrsp->accessProtectedGetRxFocus());

    pOtrsp->accessProtectedRespondToRxCommand(commandRx2); /* incoming command to go to 2 */
    CHECK(FOCUS_2 == pOtrsp->accessProtectedGetRxFocus());

    pOtrsp->accessProtectedSetRxFocus(FOCUS_1); /* current focus */
    pSo2rHardware->setMockRxSelect(MANUAL_SELECT_1);  /* switch is manually on focus 1 */
    pOtrsp->accessProtectedGetRxSwitch();
    pOtrsp->accessProtectedRespondToRxCommand(commandRx2); /* incoming command to go to 2 */
    CHECK(FOCUS_1 == pOtrsp->accessProtectedGetRxFocus()); /* ignore incoming command - switch has precedence */

    pOtrsp->accessProtectedSetRxFocus(FOCUS_2); /* current focus */
    pSo2rHardware->setMockRxSelect(MANUAL_SELECT_2);  /* switch is manually on focus 1 */
    pOtrsp->accessProtectedGetRxSwitch();
    pOtrsp->accessProtectedRespondToRxCommand(commandRx1); /* incoming command to go to 2 */
    CHECK(FOCUS_2 == pOtrsp->accessProtectedGetRxFocus()); /* ignore incoming command - switch has precedence */
}

TEST(TestableOtrsp_Setup, respondToPttCommand)
{
    /* setup */
    pSo2rHardware->setMockPttSwitch(PIN_NOT_SELECTED);
    pOtrsp->accessProtectedGetPttSwitch();
    pOtrsp->accessProtectedSetPttState(PTT_OFF); /* current ptt state */
    /* test */
    pOtrsp->accessProtectedRespondToPttCommand(commandPttOn); /* incoming command to go to PTT On */
    CHECK(PTT_ON_BY_COMMAND == pOtrsp->accessProtectedGetPttState());
    pOtrsp->accessProtectedRespondToPttCommand(commandPttOff); /* incoming command to go to Ptt Off */
    CHECK(PTT_OFF == pOtrsp->accessProtectedGetPttState());

    /* setup */
    pSo2rHardware->setMockPttSwitch(PIN_SELECTED);
    pOtrsp->accessProtectedGetPttSwitch();
    pOtrsp->accessProtectedSetPttState(PTT_ON_BY_SWITCH); /* current ptt state */
    /* test */
    pOtrsp->accessProtectedRespondToPttCommand(commandPttOn); /* incoming command to go to PTT On */
    CHECK(PTT_ON_BY_SWITCH == pOtrsp->accessProtectedGetPttState());
    pOtrsp->accessProtectedRespondToPttCommand(commandPttOff); /* incoming command to go to PTT Off*/
    CHECK(PTT_ON_BY_SWITCH == pOtrsp->accessProtectedGetPttState());
}



TEST(Otrsp_noSetup, checkForPttSwitchChange)
{
    mockGetSelectOtrsp *pOtrsp;
    MockHardware *pSo2rHardware;

    pOtrsp = new mockGetSelectOtrsp;
    pSo2rHardware = new MockHardware;
    pOtrsp->configureSo2rHardware(pSo2rHardware);

    pOtrsp->accessProtectedSetPttState(PTT_OFF);

    mock().expectOneCall("getPttSwitch").andReturnValue(PTT_ON_BY_SWITCH);
    pOtrsp->accessProtectedCheckForPttSwitchChange();
    CHECK(PTT_ON_BY_SWITCH == pOtrsp->accessProtectedGetPttState());
    mock().checkExpectations();


    mock().expectOneCall("getPttSwitch").andReturnValue(PTT_OFF);
    pOtrsp->accessProtectedCheckForPttSwitchChange();
    CHECK(PTT_OFF == pOtrsp->accessProtectedGetPttState());
    mock().checkExpectations();

    mock().clear();

    delete pSo2rHardware;
    delete pOtrsp;
}


