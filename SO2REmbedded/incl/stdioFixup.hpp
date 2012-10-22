/*
 * stdioFixup.h
 *
 *  Created on: Jun 14, 2011
 *      Author: dinkem1
 */
#ifndef STDIOFIXUP_H_
#define STDIOFIXUP_H_

#include "usart.hpp"

int  stdout_putchar (char data, FILE *stream);
int  stdin_getchar (FILE *stream);
int  stderr_putchar (char data, FILE *stream);
int  stderr_getchar (FILE *stream);
void setupStdIo(void);



#endif /* STDIOFIXUP_H_ */
