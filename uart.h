#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdint.h>

#define BAUD_PRESCALE_U2X(fosc, br) ((fosc/(br*8UL))-1)
#define BAUD_PRESCALE(fosc, br) ((fosc/(br*16UL))-1)

/* UART_FLAGS
*  NOTE: This is all configurations we need. let the rest be default:
*	data size set to 8
*	parity off
*	mode async
*	1 stop bit
*/
#define UART_TXEN	1<<0
#define UART_RXEN	1<<1
#define UART_U2X	1<<2
#define UART_UDRIE	1<<3
#define UART_TXCIE	1<<4
#define UART_RXCIE	1<<5

#define NEW_LINE uart_tx('\r'); uart_tx('\n');

#define UART_WAIT_TX while (!(UCSRA & (1<<UDRE)))

void uart_tx(uint8_t byte) {
	UART_WAIT_TX;
	UDR = byte;
}

void uart_tx_str(const char* s) {
	while (*s)
		uart_tx(*s++);
}

#define UART_WAIT_RX while (!(UCSRA & (1<<RXC)))

uint8_t uart_rx(void) {
	UART_WAIT_RX;
	return UDR;
}


void uart_init(unsigned long fosc, unsigned int br, unsigned long flags) {
	uint16_t prescaled_br;
	
	if (flags & UART_U2X) {
		prescaled_br = BAUD_PRESCALE_U2X(fosc, br);
		UCSRA |= 1<<1;
	}
	else
		prescaled_br = BAUD_PRESCALE(fosc, br);

	prescaled_br &= ~(1<<15);
	UBRRH = (prescaled_br >> 8);
	UBRRL = prescaled_br;
	
	if (flags & UART_UDRIE)
		UCSRB |= 1<<UDRIE;
	
	if (flags & UART_TXCIE)
		UCSRB |= 1<<TXCIE;
	
	if (flags & UART_RXCIE)
		UCSRB |= 1<<RXCIE;
	
	if (flags & UART_RXEN)
		UCSRB |= 1<<RXEN;
	
	if (flags & UART_TXEN)
		UCSRB |= 1<<TXEN;
}



#endif /* UART_H_ */
