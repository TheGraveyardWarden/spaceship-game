#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "keyboard.h"
#include "debugger.h"

#include "spaceshipgame.h"

int main(void)
{	
	debugger_init(F_CPU, 9600);

	printd("Debugger initialized!\r\n");

    game_init();

    while (1)
    {
        game_loop();
    }
}

// #include "interval.h"
// #include "debugger.h"

// static struct interval interval;

// static struct runner r1, r2, r3, r4;


// void _1(void* arg)
// {
//     printd("1: 1000\r\n");
// }
// void _2(void* arg)
// {
//     printd("2: 1000\r\n");
// }
// void _3(void* arg)
// {
//     printd("3: 2000\r\n");
// }
// void _4(void* arg)
// {
//     printd("4\r\n");
// }

// DEFINE_INTERVAL_ISR(1, interval)

// int main(void) {
//     int i, j, k, z;

//     debugger_init(F_CPU, 9600);
//     INTERVAL_INIT(1, interval);
//     runner_init(&r1, _1, &i, 1000, F_CPU/8);
//     runner_init(&r2, _2, &j, 1000, F_CPU/8);
//     runner_init(&r3, _3, &k, 2000, F_CPU/8);
//     runner_init(&r4, _4, &z, 4000, F_CPU/8);

//     sei();

//     printd("Debugger initialized!\r\n");

//     i = j = k = z = 0;

//     interval_add_runner(&interval, &r1);
//     interval_add_runner(&interval, &r2);
//     interval_add_runner(&interval, &r3);
//     // interval_add_runner(&interval, &r4);

//     printd("Started timer\r\n");

//     INTERVAL_RUN(1);

//     while (1);
// }


/*

MAX: 7

16  c:1,r:2
27  c:2,r:6
45  c:5,r:3

7 i passed
16  c:0
27  c:1
45  c:4

7 i passed
16  c:-1
27  c:0
45  c:3
timer counter set to 7 - 2 = 5
27 c:1,r:4

2 i passed:
16  c:1,r:2
27  c:0,r:4

7 i passed:
16  c:0
27  c:-1
timer counter set to 7 - 4 = 3
16  c:0,r:5

4 i passed:
16  c:-1,r:5
27  c:2,r6
timer countr set to 7 - 5 = 2

5


*/