#ifndef RUNNER_H_
#define RUNNER_H_

#include "types.h"

#define MAX_CNT 65535

struct counter {
	i32_t c;
	u32_t r;
};

struct counter counter_init(u32_t fosc, u32_t ms);


typedef void(*runner_cb_t)(void*);

struct runner {
    runner_cb_t cb;
    void *arg;
    struct counter counter;
    struct counter ref_counter;
};

void runner_init(struct runner* runner, runner_cb_t cb, void* arg, u32_t delay, u32_t fosc);
struct runner* runner_find_adjuster(struct runner** adjuster_runners, u8_t nr_adjusters);
void runner_adjust(struct runner* runner, u32_t factor);

inline void runner_set_counter(struct runner* runner, u32_t delay, u32_t fosc) {
    runner->ref_counter = counter_init(fosc, delay);
}

inline void runner_reset_counter(struct runner* runner) {
    runner->counter = runner->ref_counter;
}

inline void runner_set_cb(struct runner* runner, runner_cb_t cb) {
    runner->cb = cb;
}

inline void runner_set_arg(struct runner* runner, void* arg) {
    runner->arg = arg;
}

#define runner_run(runner) (runner).cb((runner).arg)


#endif /* RUNNER_H_ */