#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>


inline void timer1_init(void) {
	TIMSK |= 1<<TOIE1;
	TCNT1 = 0;
}

inline void timer1_start(void) {
	TCCR1B |= (1<<CS11);
}

inline void timer1_stop(void) {
	TCCR1B &= ~((1<<CS10) | (1<<CS11) | (1<<CS12));
}


#endif /* TIMER_H_ */