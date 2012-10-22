/*
 * stdioFixup.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: dinkem1
 *
 * 	Add support for printf
 */
#include "stdioFixup.hpp"


/*!Function     setupStdIo
*   \param      none
*   \return     void
*   \par Purpose
*   			attach USART to stdio
*   \note
*/
void
setupStdIo(void)
{
    stdout = stdin = fdevopen(stdout_putchar, stdin_getchar);
    stderr = fdevopen(stderr_putchar, NULL);
}


/*!Function     stdout_putchar
*   \param      char data
*   \param      FILE *stream
*   \return     int
*   \par Purpose
*               send a char out the UART
*   \note
*/
int
stdout_putchar (char data, FILE *stream)
{
    if (data == '\n')
    {
    	stdout_putchar('\r', stream);
    }

    /* Wait for empty transmit buffer */
    while (!( USART0_STATUS_REGISTER_A & USART_DATA_REG_IS_EMPTY));

    /* Put character into the transmit buffer */
    USART0_TX_RX_REGISTER = data;

    return 0;
}

/*!Function     stdin_getchar
*   \param      FILE *stream
*   \return     int
*   \par Purpose
*               not implemented - get a char from the UART
*   \note
*/
int
stdin_getchar (FILE *stream)
{
    return 0;
}


/*!Function     stderr_putchar
*   \param      char data
*   \param      FILE *stream
*   \return     int
*   \par Purpose
*               send a char out the UART
*   \note
*/
int
stderr_putchar (char data, FILE *stream)
{
    //printf("%c", data);
    if (data == '\n')
    {
        stderr_putchar('\r', stream);
    }

    /* Wait for empty transmit buffer */
    while (!( USART1_STATUS_REGISTER_A & USART_DATA_REG_IS_EMPTY));

    /* Put character into the transmit buffer */
    USART1_TX_RX_REGISTER = data;

    return 0;
}



