/*
 * uart.cpp
 *
 *  Created on: Jun 22, 2011
 *      Author: dinkem1
 */
/*----------------------------- Nested includes -----------------------------*/
#include "usart.hpp"
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
/*--------------------------- Function prototypes ---------------------------*/


/*!Function     Usart::Usart
*   \return     none
*   \par Purpose
*               constructor
*/
Usart::Usart():
pUsartRx(0),
pUsartTx(0)
{;}


/*!Function     Usart::configureTx
*   \return     none
*   \par Purpose
*               tell this uart what Tx object to use
*/
void
Usart::configureTx(UsartTx *usartTx)
{
    pUsartTx = usartTx;
}

/*!Function     Usart::configureRx
*   \return     none
*   \par Purpose
*               tell this uart what Rx object to use
*/
void
Usart::configureRx(UsartRx *usartRx)
{
    pUsartRx = usartRx;
}

/*!Function     Usart::getRx
*   \return     UsartRx *
*   \par Purpose
*				return a pointer to the uart RX object
*/
UsartRx *
Usart::getRx(void)
{
    return pUsartRx;
}

/*!Function     Usart::getTx
*   \return     UsartTRx *
*   \par Purpose
*               return a pointer to the uart TX object
*/
UsartTx *
Usart::getTx(void)
{
    return pUsartTx;
}


/*!Function     UsartTx::UsartTx
*   \return     none
*   \par Purpose
*               constructor - create a uart Tx object
*/
UsartTx::UsartTx()
{;}


/*!Function     UsartTx::sendOutGoingData0
*   \param      char *buff
*   \return     uint8_t
*   \par Purpose
*
*/
uint8_t
UsartTx::sendOutGoingData0(char *buff)
{
    static const int8_t maxBuffSize = 40;
    uint8_t dataCount = strlen(buff);
    uint8_t sentCount = 0;

    memset(txShadowBuffer, 0x00, maxBuffSize);

    for(uint8_t i = 0; i < dataCount; ++i)
    {
        /* Wait for empty transmit buffer */
        while (!( USART0_STATUS_REGISTER_A & USART_DATA_REG_IS_EMPTY));

        /* Put character into the transmit buffer */
        USART0_TX_RX_REGISTER = buff[i];
        /* update shadow Buffer */
        txShadowBuffer[sentCount] = buff[i];
        ++sentCount;
    }
    /* Wait for empty transmit buffer */
    while (!( USART0_STATUS_REGISTER_A & USART_DATA_REG_IS_EMPTY));
    /* Send carriage return character */
    USART0_TX_RX_REGISTER = '\r';
    txShadowBuffer[sentCount] = 0x00;
    ++sentCount;

    return sentCount;
}

/*!Function     UsartRx::UsartRx
*   \return     none
*   \par Purpose
*               constructor - create a uart Rx object
*/
UsartRx::UsartRx():
pIncomingData(0),
nextIncomingBuffer(0),
nextBufferToProcess(0)
{
    for(uint8_t i = 0; i < MAX_INCOMING_BUFFERS; ++i)
    {
        incomingData[i].count = 0;
        incomingData[i].ready = false;
    }
    pIncomingData = &incomingData[0];
}


/*!Function     UsartRx::rxInterrupt
*   \return     none
*   \par Purpose
*               actual functionality for the USART Rx ISR
*   \note		we are using a triple buffer scheme. there
*   			is an array of three buffers. Once one is ready
*   			(complete string with eos), the next one starts being filled.
*
*/
void
UsartRx::rxInterrupt(uint8_t data)
{
    /* Special handling for return, means end of command */
    if ('\r' == data)
    {
        /* close out this buffer */
        pIncomingData->buffer[pIncomingData->count] = 0x00;
        pIncomingData->ready = true;

        /* setup next buffer */
        if(++nextIncomingBuffer > MAX_INCOMING_BUFFERS-1)
        {
            nextIncomingBuffer = 0;
        }
        pIncomingData = &incomingData[nextIncomingBuffer];
        pIncomingData->count = 0;
        pIncomingData->ready = false;
    }
    else
    {
        if (pIncomingData->count < (MAX_UART_BUFFER - 2))
        {
            pIncomingData->buffer[pIncomingData->count] = data;
            ++pIncomingData->count;
        }
        else
        {
            ; /* error - what should we do here? */
        }
    }
}

/*!Function     UsartRx::checkForIncomingData
*   \return     uint8_t *  (no data returns zero)
*   \par Purpose
*               return a pointer to the next valid command
*               (to be processed by others)
*/
uint8_t *
UsartRx::checkForIncomingData(void)
{
    incomingDataBuffer *pIncoming = 0;
    uint8_t *pData = 0;
    uint8_t i = nextBufferToProcess;

    for(uint8_t count = 0; count < MAX_INCOMING_BUFFERS ; ++i, ++count)
    {
        if(i > MAX_INCOMING_BUFFERS-1)
        {
            i = 0;
        }
        pIncoming = &incomingData[i];

        if(true == pIncoming->ready)
        {
            pData = pIncoming->buffer;
            pIncoming->ready = false;
            //printf("uartCheckForIncomingData buffer:%d\n", i);
            /* setup for next buffer */
            if(++nextBufferToProcess > MAX_INCOMING_BUFFERS-1)
            {
                nextBufferToProcess = 0;
            }
            break;
        }
    }
    return pData;
}

/*!Function     UsartRx::getIncomingDataBuffer
*   \return     uint8_t *  (no data returns zero)
*   \par Purpose
*               return a pointer to the specified data buffer
*/
incomingDataBuffer *
UsartRx::getIncomingDataBuffer(uint8_t buffNum)
{
    return &incomingData[buffNum];
}

/*!Function     UsartRx::initUsart0
*   \param      uint16_t baud
*   \return     void
*   \par Purpose
*               initialize the USART0
*   \note
*/
void
Usart::initUsart0(uint16_t baud)
{
    USART0_STATUS_REGISTER_A = USART_DOUBLE_TX_SPEED;
    baud = (F_CPU / 4 / baud - 1) / 2;

    /*Set baud rate */
    USART0_BAUD_RATE_HIGH_BYTE = (unsigned char)(baud>>8);
    USART0_BAUD_RATE_LOW_BYTE = (unsigned char)baud;

    /* Enable receiver and transmitter */
    USART0_STATUS_REGISTER_B = USART_RX_ENABLE | USART_TX_ENABLE | USART_RX_INTERRUPT_ENABLE;

    /* Set frame format: 8data, 1stop bit */
    USART0_STATUS_REGISTER_C = USART_8BIT;
}


/*!Function     UsartRx::initUsart1
*   \param      uint16_t baud
*   \return     void
*   \par Purpose
*               initialize the USART1
*   \note
*/
void
Usart::initUsart1(uint16_t baud)
{
    USART1_STATUS_REGISTER_A = USART_DOUBLE_TX_SPEED;
    baud = (F_CPU / 4 / baud - 1) / 2;

    /*Set baud rate */
    USART1_BAUD_RATE_HIGH_BYTE = (unsigned char)(baud>>8);
    USART1_BAUD_RATE_LOW_BYTE = (unsigned char)baud;

    /* Enable transmitter */
    USART1_STATUS_REGISTER_B = USART_TX_ENABLE;

    /* Set frame format: 8data, 1stop bit */
    USART1_STATUS_REGISTER_C = USART_8BIT;
}




