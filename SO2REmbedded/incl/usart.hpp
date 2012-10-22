/*
 * uart.hpp
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>


#define MAX_UART_BUFFER     64
#define MAX_INCOMING_BUFFERS 3

#define USART0_TX_RX_REGISTER           UDR0
#define USART0_STATUS_REGISTER_A        UCSR0A
#define USART0_STATUS_REGISTER_B        UCSR0B
#define USART0_STATUS_REGISTER_C        UCSR0C
#define USART0_BAUD_RATE_HIGH_BYTE      UBRR0H
#define USART0_BAUD_RATE_LOW_BYTE       UBRR0L

#define USART1_TX_RX_REGISTER           UDR1
#define USART1_STATUS_REGISTER_A        UCSR1A
#define USART1_STATUS_REGISTER_B        UCSR1B
#define USART1_STATUS_REGISTER_C        UCSR1C
#define USART1_BAUD_RATE_HIGH_BYTE      UBRR1H
#define USART1_BAUD_RATE_LOW_BYTE       UBRR1L

#define USART_RX_COMPLETE          (1<<RXC0)
#define USART_DOUBLE_TX_SPEED      (1<<U2X0)
#define USART_RX_ENABLE            (1<<RXEN0)
#define USART_TX_ENABLE            (1<<TXEN0)
#define USART_RX_INTERRUPT_ENABLE  (1<<RXCIE0)
#define USART_8BIT                 (3<<UCSZ00)
#define USART_DATA_REG_IS_EMPTY    (1<<UDRE0)


struct incomingDataBuffer
{
    uint8_t buffer[MAX_UART_BUFFER];
    uint8_t count;
    bool    ready;
};


class UsartTx
{
public:
    UsartTx();
    virtual ~UsartTx(){;}

    virtual uint8_t sendOutGoingData0(char *buff);
    virtual char *getTxShadowBuffer(void){return txShadowBuffer;}

private:
    static const int8_t maxBuffSize = 40;
    char txShadowBuffer[maxBuffSize];
};



class UsartRx
{
public:
    UsartRx();
    virtual ~UsartRx(){;}

    virtual void rxInterrupt(uint8_t data);
    virtual uint8_t *checkForIncomingData(void);
    virtual incomingDataBuffer *getIncomingDataBuffer(uint8_t buff);

private:
    incomingDataBuffer incomingData[MAX_INCOMING_BUFFERS];
    incomingDataBuffer *pIncomingData;
    uint8_t nextIncomingBuffer;
    uint8_t nextBufferToProcess;
};


class Usart
{
public:
    Usart();
    virtual ~Usart(){;}

    virtual void configureRx(UsartRx *usartRx);
    virtual void configureTx(UsartTx *usartTx);
    virtual UsartRx *getRx(void);
    virtual UsartTx *getTx(void);

    virtual void initUsart0(uint16_t baud);
    virtual void initUsart1(uint16_t baud);


protected:
    UsartRx *pUsartRx;
    UsartTx *pUsartTx;
};




#endif /* UART_H_ */
