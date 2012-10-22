/*
 * otrsp.hpp
 *
 *  Created on: Jul 8, 2011
 *      Author: dinkem1
 */
#ifndef OTRSP_HPP_
#define OTRSP_HPP_

#include "stdint.h"
#include "so2rHardware.hpp"
#include "hardwareInterface.hpp"


#define MAX_RESPONSE    40

enum otrspEnumeration
{
    AUX,
    BAND,
    CR0,
    ECR,
    ERX,
    ETX,
    FW,
    MAP,
    MODE,
    NAME,
    RX,
    TX,
    RAWQUERY,
    UNSUPPORTED,
    UNKNOWN,
    LAST_INSTRUCTION
};


class Otrsp
{
public:
    Otrsp();
    virtual ~Otrsp(){;}

    virtual char *processOtrspCommunications(char *pData);
    virtual void processOtrspHardware(void);

    /* hardware */
    virtual void configureSo2rHardware(HardwareInterface *newSo2r){pSo2rHardware = newSo2r;}
    virtual HardwareInterface *getSo2rHardware(void){return pSo2rHardware;}


protected:
    uint8_t txFocus;        //which Tx has the focus
    uint8_t rxFocus;        //which Rx has the focus
    uint8_t pttState;       //ptt state
    HardwareInterface *pSo2rHardware;

    virtual bool parseCommandForQuery(char *commandString);
    virtual otrspEnumeration parseQuery(char *queryString);
    virtual otrspEnumeration parseCommand(char *instructionString);

    virtual char *respondToCommand(otrspEnumeration command, char *pCommand);
    virtual char *respondToQuery(otrspEnumeration query, char *pQuery);
    virtual char *respondToQueryName(void);
    virtual char *respondToPing(void);
    virtual char *respondToUnsupportedQuery(char *pQuery);
    virtual char *respondToQueryTx(char *pQuery);
    virtual char *respondToQueryRx(char *pQuery);
    virtual char *respondToQueryPtt(char *pQuery);


    virtual char *respondToTxCommand(char *pCommand);
    virtual char *respondToRxCommand(char *pCommand);
    virtual char *respondToPttCommand(char *pCommand);

    virtual void checkRxSelectSwitch(void);
    virtual void checkTxSelectSwitch(void);
    virtual void checkForPttSwitchChange(void);
    virtual uint8_t getRxSwitch(void);
    virtual uint8_t getTxSwitch(void);
    virtual uint8_t getPttSwitch(void);
    virtual void setRxFocus(uint8_t newFocus);
    virtual void setTxFocus(uint8_t newFocus);
    virtual void setPttState(uint8_t newState);
    virtual uint8_t getRxFocus(void){return rxFocus;}
    virtual uint8_t getTxFocus(void){return txFocus;}
    virtual uint8_t getPttState(void){return pttState;}

};




#endif /* OTRSP_HPP_ */
