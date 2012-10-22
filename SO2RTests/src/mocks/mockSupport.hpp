/*
 * mockSupport.hpp
 *
 *  Created on: Jul 20, 2011
 *      Author: dinkem1
 */

#ifndef MOCKSUPPORT_HPP_
#define MOCKSUPPORT_HPP_

#define MAX_PIN_ARRAY 60

extern "C"
{
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t data);
int digitalRead(uint8_t pin);
void clearPortArrays(void);
void setMockMillis(uint32_t newMillis);

} //extern c


#endif /* MOCKSUPPORT_HPP_ */
