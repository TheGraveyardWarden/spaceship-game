#ifndef _INTERVAL_H
#define _INTERVAL_H

#include "types.h"
#include "timer.h"
#include "runner.h"


#define MAX_RUNNERS 15

// interval

struct interval {
    struct runner* runners[MAX_RUNNERS];
    u8_t nr_runners;
};

void interval_init(struct interval* interval);
i8_t interval_add_runner(struct interval* interval, struct runner* runner);
i8_t interval_remove_runner(struct interval* interval, struct runner* runner);


// initializes interval for timer @n
#define INTERVAL_INIT(n, interval)  \
    interval_init(&interval);  \
    timer##n##_init()

// runs interval for timer @n
#define INTERVAL_RUN(n) timer##n##_start()

// stops interval for timer @n
#define INTERVAL_STOP(n) timer##n##_stop()

// defines interval isr for timer @n
#define DEFINE_INTERVAL_ISR(n, interval)                                       \
ISR(TIMER##n##_OVF_vect) {                                                     \
    u8_t i, nr_adjusters;                                                      \
    i32_t *c;                                                                  \
    struct runner* adjuster_runners[interval.nr_runners];                      \
    struct runner *adjuster;                                                   \
    \
    nr_adjusters = 0;                                                          \
    \
    for (i = 0; i < interval.nr_runners; i++) {                                \
        c = &(interval.runners[i]->counter.c);                                 \
    \
        if (*c > 0) {(*c)--; continue;}                                        \
    \
        if (*c == 0) {                                                         \
            adjuster_runners[nr_adjusters++] = interval.runners[i];            \
            (*c)--;                                                            \
            continue;                                                          \
        }                                                                      \
    \
        runner_reset_counter(interval.runners[i]);                             \
        runner_run(*interval.runners[i]);                                      \
    }                                                                          \
    \
    if (nr_adjusters > 0) {                                                    \
        adjuster = runner_find_adjuster(adjuster_runners, nr_adjusters);       \
        \
        TCNT##n = MAX_CNT - adjuster->counter.r;                               \
        \
        for(i = 0; i < interval.nr_runners; i++) {                             \
            if (interval.runners[i] == adjuster){                              \
                continue;                                                      \
            }                                                                  \
        \
            runner_adjust(interval.runners[i], MAX_CNT - adjuster->counter.r); \
        }                                                                      \
    }                                                                          \
}

#endif /* _INTERVAL_H */



/*
65535

1: 78000       -> c: 1, r: 12465
2: 120000      -> c: 1, r: 54465

65535 instructions passed:
1st -> c1: 0, c2: 0+1 --- r2: 42000

12465 instructions passed:
2nd -> c1: -1, c2: 0 ---
reset c1: 1 and r1: 12465 -> needed ins -> 78000
runner 1 needs re-adjust: F(c) -> c*MAX+r - 42000 = y -> r: y mod MAX ; c: (y / MAX)+1;

42000 instructions passed:
c2 -> -1; calls and resets -> c2: 1; r2: 54465 -> needed ins: 120000
c1 -> 0; needs to adjust TCNT
3rd -> c1: ?, c2: -1 --- 
*/

/*
NOTE:
    everytime we re-adjust TCNT we need to re-calc all runners remainder;
*/

/*

#define MAX 10

runner_1: 27ins  -> c1:  2, r1: 7
runner_2: 32ins  -> c2:  3, r2: 2
runner_3: 146ins -> c3: 14, r3: 6
runner_4: 145ins -> c4: 14, r4: 5

10 ins passed:
runner_1: c1:  1, r1: 7;
runner_2: c2:  2, r2: 2;
runner_3: c3: 13, r3: 6
runner_4: c4: 13, r4: 5

10 ins passed:
runner_1: c1: 0, r1: 7;
runner_2: c2: 1, r2: 2;
runner_3: c3: 12, r3: 6
runner_4: c4: 12, r4: 5

set TCNT to MAX-r1 = 3;

7 ins passed:
runner_1: c1: -1, r1: 7;
runner_2: c2:  0, r2: 2;

set TCNT r1-r2:  7 - 2 = 5;

5 ins passed:
runner_1: c1:  2, r1: 2;
runner_2: c2: -1, r2: 2;

*/
