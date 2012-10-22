/*
 * hardwareInterface.hpp
 *
 *  Created on: May 19, 2011
 *      Author: mwdink
 */

#ifndef HARDWAREINTERFACE_HPP_
#define HARDWAREINTERFACE_HPP_
#include <stdint.h>

class HardwareInterface
{
public:
    virtual uint8_t getTxSwitch(void) = 0;
    virtual uint8_t getRxSwitch(void) = 0;
    virtual uint8_t getPttSwitch(void) = 0;
    virtual void setTxFocusHardware(uint8_t txSelect) = 0;
    virtual void setRxFocusHardware(uint8_t rxSelect) = 0;
    virtual void setPttHardware(uint8_t pttState) = 0;

protected:
    virtual void configureLedDrivePins(void) = 0;
    virtual void configureTxSelectSensePins(void) = 0;
    virtual void configureRxSelectSensePins(void) = 0;
    virtual void configurePttSelectSensePins(void) = 0;

private:

};
#endif /* HARDWAREINTERFACE_HPP_ */
