/*
 * controlHardware.hpp
 *
 *  Created on: May 19, 2011
 *      Author: mwdink
 */

#ifndef SO2RHARDWARE_HPP
#define SO2RHARDWARE_HPP

#include "hardwareInterface.hpp"

#define SELECT_TX1_PIN     54   /* A0 */
#define SELECT_TX2_PIN     55   /* A1 */
#define SELECT_RX1_PIN     56   /* A2 */
#define SELECT_RX2_PIN     57   /* A3 */
#define SELECT_PTT_PIN     58   /* A4 */

/* these correspond to the number on the arduino pcb */
#define TX1_LED_PIN     9
#define TX2_LED_PIN     8
#define RX1_LED_PIN     7
#define RX2_LED_PIN     6
#define PTT_LED_PIN     4

/* active LOW pins */
#define PIN_SELECTED                    0
#define PIN_NOT_SELECTED                1

#define FOCUS_1                         1
#define FOCUS_2                         2
#define MANUAL_SELECT_AUTO 				0
#define MANUAL_SELECT_1 				FOCUS_1
#define MANUAL_SELECT_2 				FOCUS_2
#define MANUAL_SELECT_PENDING_DEBOUNCE 	3

#define PTT_OFF                         PIN_NOT_SELECTED
#define PTT_ON_BY_SWITCH                PIN_SELECTED
#define PTT_PENDING_DEBOUNCE            2
#define PTT_ON_BY_COMMAND               3

class So2rHardware: public HardwareInterface
{
public:
    So2rHardware();
    virtual ~So2rHardware(){;}

    uint8_t getTxSwitch(void);
    uint8_t getRxSwitch(void);
    uint8_t getPttSwitch(void);
    void setTxFocusHardware(uint8_t txFocus);
    void setRxFocusHardware(uint8_t rxFocus);
    void setPttHardware(uint8_t pttState);

protected:
    void configureLedDrivePins(void);
    void configureTxSelectSensePins(void);
    void configureRxSelectSensePins(void);
    void configurePttSelectSensePins(void);


private:

};


#endif /* SO2RHARDWARE_HPP */
