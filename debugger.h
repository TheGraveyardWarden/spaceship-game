#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include "uart.h"
#include <stdarg.h>
#include <stdio.h>

inline void debugger_init(unsigned long fosc , unsigned int br) {
	uart_init(fosc, br, UART_TXEN | UART_U2X);
}

void printd(const char* fmt, ...) {
	va_list ap;
	char s[255];
	int d;

	va_start(ap, fmt);

	while (*fmt) {
		if (*fmt != '%') {
			uart_tx(*fmt++);
		}
		else {
			switch (*++fmt) {
				case 's':
					sprintf(s, "%s", va_arg(ap, char*));
					uart_tx_str(s);
					fmt++;
					break;
				case 'd':
					d = va_arg(ap, int);
					sprintf(s, "%d", d);
					uart_tx_str(s);
					fmt++;
					break;
				case 'x':
					d = va_arg(ap, int);
					sprintf(s, "0x%x", d);
					uart_tx_str(s);
					fmt++;
					break;
			}
		}
	}
}

#endif /* DEBUGGER_H_ */