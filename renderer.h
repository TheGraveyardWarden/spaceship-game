#ifndef RENDERER_H_
#define RENDERER_H_

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#ifdef CONFIG_DEBUG_RENDERER
#include "debugger.h"
#endif

#include "vec.h"

typedef char i8_t;
typedef unsigned char u8_t;

#define SR0			PORTC
#define SR0DS		2
#define SR0INCLK	3
#define SR0OUTCLK	4
#define DDRSR0      DDRC

#define SR1			PORTA
#define SR1DS		0
#define SR1INCLK	1
#define SR1OUTCLK	2
#define DDRSR1      DDRA

#define SR2			PORTC
#define SR2DS		5
#define SR2INCLK	6
#define SR2OUTCLK	7
#define DDRSR2      DDRC

#define SR3			PORTA
#define SR3DS		3
#define SR3INCLK	4
#define SR3OUTCLK	5
#define DDRSR3      DDRA

#define INIT_SR_PORTS(n) DDRSR##n |= (1 << SR##n##DS) | (1 << SR##n##INCLK) | (1 << SR##n##OUTCLK)

#define ROW			PORTB
#define DDRROW      DDRB

enum monitor
{
	MONITOR_0 = 0,
	MONITOR_1,
	MONITOR_2,
	MONITOR_3
};

#ifndef MONITOR_LAYOUT_1X4
#ifndef MONITOR_LAYOUT_2X1
	#define MONITOR_LAYOUT_2X2
	#define NR_MONITORS 4
#else  /* MONITOR_LAYOUT_2X1 */
	#define NR_MONITORS 2
#endif /* MONITOR_LAYOUT_2X1 */
#else  /* MONITOR_LAYOUT_1X4 */
	#define NR_MONITORS 4
#endif /* MONITOR_LAYOUT_1X4 */

#define NR_ROWS     8
#define data_at(i, j)   data[(i * NR_ROWS) + j]
#define ON 1
#define OFF 0
#define MOD8(x) (x)%8

#define SEND_BYTE_AND_ACTIVATE(data, N)		\
	_SEND_BYTE_AND_ACTIVATE(data, N)

#define _SEND_BYTE_AND_ACTIVATE(data, N)	\
	_SEND_BYTE_AND_ACTIVATE_FOR_##N(data)

#define _SEND_BYTE_AND_ACTIVATE_FOR_1(data)	\
	send_data_and_activate(data, MONITOR_0)

#define _SEND_BYTE_AND_ACTIVATE_FOR_2(data)	\
	_SEND_BYTE_AND_ACTIVATE_FOR_1(data);	\
	send_data_and_activate(data, MONITOR_1)

#define _SEND_BYTE_AND_ACTIVATE_FOR_3(data)	\
	_SEND_BYTE_AND_ACTIVATE_FOR_2(data);	\
	send_data_and_activate(data, MONITOR_2)

#define _SEND_BYTE_AND_ACTIVATE_FOR_4(data)	\
	_SEND_BYTE_AND_ACTIVATE_FOR_3(data);	\
	send_data_and_activate(data, MONITOR_3)

#define SEND_DATA_AT_AND_ACTIVATE(N)					\
	_SEND_DATA_AT_AND_ACTIVATE(N)

#define _SEND_DATA_AT_AND_ACTIVATE(N)					\
	_SEND_DATA_AT_AND_ACTIVATE_FOR_##N()

#define _SEND_DATA_AT_AND_ACTIVATE_FOR_1()				\
	send_data_and_activate(~data_at(0, i), MONITOR_0)

#define _SEND_DATA_AT_AND_ACTIVATE_FOR_2()				\
	_SEND_DATA_AT_AND_ACTIVATE_FOR_1();					\
	send_data_and_activate(~data_at(1, i), MONITOR_1)

#define _SEND_DATA_AT_AND_ACTIVATE_FOR_3()				\
	_SEND_DATA_AT_AND_ACTIVATE_FOR_2();					\
	send_data_and_activate(~data_at(2, i), MONITOR_2)

#define _SEND_DATA_AT_AND_ACTIVATE_FOR_4()				\
	_SEND_DATA_AT_AND_ACTIVATE_FOR_3();					\
	send_data_and_activate(~data_at(3, i), MONITOR_3)

#define _send_bit(bit, n)					\
	if (bit)	SR##n |= (1 << SR##n##DS);	\
	else		SR##n &= ~(1 << SR##n##DS);	\
	SR##n |= (1 << SR##n##INCLK);			\
	SR##n &= ~(1 << SR##n##INCLK)


void send_bit(i8_t bit, enum monitor monitor_select)
{
	switch(monitor_select)
	{
		case MONITOR_0:
			_send_bit(bit, 0);
			break;
		case MONITOR_1:
			_send_bit(bit, 1);
			break;
		case MONITOR_2:
			_send_bit(bit, 2);
			break;
		case MONITOR_3:
			_send_bit(bit, 3);
			break;
	}
}

#define _active_out(n)				\
	SR##n |= (1 << SR##n##OUTCLK);	\
	SR##n &= ~(1 << SR##n##OUTCLK)

void active_out(enum monitor monitor_select)
{
	switch(monitor_select)
	{
		case MONITOR_0:
			_active_out(0);
			break;
		case MONITOR_1:
			_active_out(1);
			break;
		case MONITOR_2:
			_active_out(2);
			break;
		case MONITOR_3:
			_active_out(3);
			break;
	}
}

void send_data(i8_t data, enum monitor monitor_select)
{
	for (int i = 0; i < 8; i++)
		send_bit(data & (1 << i), monitor_select);
}

void send_data_and_activate(i8_t data, enum monitor monitor_select)
{
	send_data(data, monitor_select);
	active_out(monitor_select);
}

// i8_t data[NR_ROWS * NR_MONITORS] = {
//     0b00000000, 0b00111100, 0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00111100, 0b00000000,
//     0b00000000, 0b00001000, 0b00011000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00000000,
//     0b00000000, 0b00111100, 0b00000100, 0b00000100, 0b00111100, 0b00100000, 0b00111100, 0b00000000,
//     0b00000000, 0b00111100, 0b00000100, 0b00111100, 0b00000100, 0b00111100, 0b00000000, 0b00000000
// };
i8_t data[NR_ROWS * NR_MONITORS] = { 0 };

enum monitor get_monitor(u8_t x, u8_t y) {
	#ifdef MONITOR_LAYOUT_2X2
    return x < 8 ? (
        y < 8 ? MONITOR_0 : MONITOR_2
    ) : (
        y < 8 ? MONITOR_1 : MONITOR_3
    );
	#endif /* MONITOR_LAYOUT_2X2 */
	
	#ifdef MONITOR_LAYOUT_1X4
	return x < 8 ? MONITOR_0 : x < 16 ? MONITOR_1 : x < 24 ? MONITOR_2 : MONITOR_3;
	#endif /* MONITOR_LAYOUT_1X4 */
	
	#ifdef MONITOR_LAYOUT_2X1
	return y < 8 ? MONITOR_0 : MONITOR_1;
	#endif /* MONITOR_LAYOUT_2X1 */
}

void renderer_init(void)
{
    DDRROW = 0xff;
    INIT_SR_PORTS(0);
    INIT_SR_PORTS(1);
    INIT_SR_PORTS(2);
    INIT_SR_PORTS(3);

    ROW = 0x00;
}

void renderer_draw(void)
{
    int i;
    
    for (i = 0; i < NR_ROWS; i++)
    {
		SEND_BYTE_AND_ACTIVATE(0xff, NR_MONITORS);
        ROW = (1 << i);
        SEND_DATA_AT_AND_ACTIVATE(NR_MONITORS);
    }
}

void renderer_set_data_arr(vec2_t* points, u8_t n)
{
	vec2_t* tmp;
	enum monitor monitor;
	u8_t _x, _y;

	while (n--)
	{
		tmp = points + n;
		_x = MOD8(tmp->x); _y = MOD8(tmp->y);
    	monitor = get_monitor(tmp->x, tmp->y);
		data_at(monitor, _y)  |= (0x80 >> _x);
	}
}

void renderer_set_data(u8_t x, u8_t y, i8_t value)
{
    enum monitor monitor;
    u8_t _x, _y;

    _x = MOD8(x); _y = MOD8(y);
    monitor = get_monitor(x, y);
    if (value) data_at(monitor, _y)  |= (0x80 >> _x);
    else data_at(monitor, _y)        &= ~(0x80 >> _x);
}

void renderer_clear_data(void)
{
    int i;
    for (i = 0; i < NR_ROWS * NR_MONITORS; i++)
        data[i] = 0;
}

#endif /* RENDERER_H_ */