#include "runner_pool.h"
#include "types.h"

void runner_pool_init(struct runner_pool* pool) {
	int i;

	for (i = 0; i < MAX_RUNNER_POOL; i++) {
		pool->runners[i].is_idle = 1;
	}
}

struct runner* runner_pool_add(struct runner_pool* pool, runner_cb_t f, void* arg, u32_t delay, u32_t fosc) {
	struct p_runner* runner;
	int i;

	for (i = 0; i < MAX_RUNNER_POOL; i++) {
		runner = &pool->runners[i];
		if (runner->is_idle) {
			break;
		}
	}

	if (!(runner->is_idle)) {
		return NULL;
	}

	runner->is_idle = 0;
	runner_init(&runner->runner, f, arg, delay, fosc);
	
	return &runner->runner;
}

void runner_pool_remove(struct runner_pool* pool, struct runner* runner) {
	struct p_runner* p_runner;

	p_runner = container_of(runner, struct p_runner, runner);
	p_runner->is_idle = 1;
}