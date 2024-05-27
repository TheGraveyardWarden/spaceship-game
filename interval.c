#include "interval.h"

#include "types.h"
#include "error.h"
#include "runner.h"

void interval_init(struct interval* interval) {
    interval->nr_runners = 0;
}

i8_t interval_add_runner(struct interval* interval, struct runner* runner) {
    if (interval->nr_runners == MAX_RUNNERS) {
        return -ENOCAP;
    }

    interval->runners[interval->nr_runners++] = runner;

    return 0;
}

i8_t interval_remove_runner(struct interval* interval, struct runner* runner) {
    u8_t i, j;

    for (i = 0; i < interval->nr_runners; i++) {
        if (interval->runners[i] == runner) {
            interval->nr_runners--;

            for (j = i; j < interval->nr_runners; j++) {
                interval->runners[j] = interval->runners[j+1];
            }

            return 0;
        }
    }

    return -ENOEXIST;
}
