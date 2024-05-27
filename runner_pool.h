#ifndef RUNNER_POOL_H_
#define RUNNER_POOL_H_

#include "types.h"
#include "runner.h"


#define MAX_RUNNER_POOL 10

struct p_runner {
	struct runner runner;
	u8_t is_idle;
};

struct runner_pool {
	struct p_runner runners[MAX_RUNNER_POOL];
};

void runner_pool_init(struct runner_pool* pool);
struct runner* runner_pool_add(struct runner_pool* pool, runner_cb_t f, void* arg, u32_t delay, u32_t fosc);
void runner_pool_remove(struct runner_pool* pool, struct runner* runner);



#endif /* RUNNER_POOL_H_ */