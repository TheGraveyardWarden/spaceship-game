#include "runner.h"


struct counter counter_init(u32_t fosc, u32_t ms) {
	struct counter counter;
	u32_t t;

	t = ((ms/1000.0)*(float)(fosc));
 
	counter.r = t % MAX_CNT;
	counter.c = (t / MAX_CNT)-1;

	return counter;
}


void runner_init(struct runner* runner, runner_cb_t cb, void* arg, u32_t delay, u32_t fosc) {
    runner->cb = cb;
    runner->arg = arg;
    runner->counter = runner->ref_counter = counter_init(fosc, delay);
}

struct runner* runner_find_adjuster(struct runner** adjuster_runners, u8_t nr_adjusters) {
    u8_t i, j;
    u32_t min;

    min = adjuster_runners[0]->counter.r;
    j = 0;

    for (i = 0; i < nr_adjusters; i++) {
        if (adjuster_runners[i]->counter.r < min) {
            min = adjuster_runners[i]->counter.r;
            j = i;
        }
    }

    return adjuster_runners[j];
}

void runner_adjust(struct runner* runner, u32_t factor) {
    if ((runner->counter.r + factor) >= MAX_CNT) {
        runner->counter.c++;
        runner->counter.r = runner->counter.r + factor - MAX_CNT;
    } else {
        runner->counter.r += factor;
    }
}