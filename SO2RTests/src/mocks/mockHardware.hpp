/*
 * mockHardware.hpp
 *
 *  Created on: Jun 21, 2011
 *      Author: dinkem1
 */

#include "hardwareInterface.hpp"


class MockHardware: public HardwareInterface
{
public:
    MockHardware();
    virtual ~MockHardware(){;}

    uint8_t getTxSwitch(void){return txSelect;}
    uint8_t getRxSwitch(void){return rxSelect;}
    uint8_t getPttSwitch(void){return pttSwitch;}
    void setMockTxSelect(uint8_t select){txSelect = select;}
    void setMockRxSelect(uint8_t select){rxSelect = select;}
    void setMockPttSwitch(uint8_t newSetting){pttSwitch = newSetting;}

protected:
    uint8_t txSelect;
    uint8_t rxSelect;
    uint8_t pttSwitch;


private:
    void configureLedDrivePins(void){;}
    void configureTxSelectSensePins(void){;}
    void configureRxSelectSensePins(void){;}
    void configurePttSelectSensePins(void){;}
    void setTxFocusHardware(uint8_t focus){;}
    void setRxFocusHardware(uint8_t focus){;}
    void setPttHardware(uint8_t pttSelect){;}

};


