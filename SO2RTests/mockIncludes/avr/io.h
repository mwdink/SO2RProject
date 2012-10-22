#ifndef IO_H_
#define IO_H_

extern uint8_t udr;
extern uint8_t ucsra;
extern uint8_t ubrrh;
extern uint8_t ubrrl;
extern uint8_t ucsrb;
extern uint8_t ucsrc;
extern uint8_t porth;

#define UDR0  udr
#define UCSR0A	ucsra
#define UBRR0H  ubrrh
#define UBRR0L  ubrrl
#define UCSR0B  ucsrb
#define UCSR0C  ucsrc

#define UCSR1A  ucsra
#define UBRR1H  ubrrh
#define UBRR1L  ubrrl
#define UCSR1B  ucsrb
#define UCSR1C  ucsrc

#define F_CPU   16000000
#define U2X0    1
#define RXEN0   4
#define TXEN0   3
#define RXCIE0  7
#define UCSZ00  1
#define RXC0  0
#define UDRE0 0


//#define PORTH  portH
//#define PH7    7
//#define PH6    6
//#define PH5    5
//#define PH4    4
//#define PH3    3
//#define PH2    2
//#define PH1    1
//#define PH0    0



#endif /* IO_H_ */
