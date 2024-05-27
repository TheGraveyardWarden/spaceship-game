#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <avr/io.h>
#include "uart.h"

inline void keyboard_init(unsigned long fosc, unsigned int br) {
	uart_init(fosc, br, UART_RXEN | UART_U2X | UART_RXCIE);
}

#define DEFINE_KEYBOARD_ISR(direction) 	\
ISR(USART_RXC_vect) {		\
	uint8_t ret = UDR;		\
							\
	switch (ret) {			\
		case 'w':			\
			direction = UP;	\
			break;			\
		case 's':			\
			direction = DOWN;	\
			break;			\
		case 'a':			\
			direction = LEFT;	\
			break;			\
		case 'd':			\
			direction = RIGHT;	\
			break;			\
	}						\
}

#endif /* KEYBOARD_H_ */