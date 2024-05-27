#ifndef SPACESHIPGAMETYPES_H_
#define SPACESHIPGAMETYPES_H_

#include "spaceshipgametypes.h"

#include "ship.h"
#include "types.h"

enum game_status
{
	RUNNING = 0,
	LOST
};

struct game {
	struct ship me;
	vec(enemy_ship, enemies, MAX_NR_ENEMIES);
	u8_t nr_alive_enemies, nr_dead_enemies;

	enum game_status status;
};



static struct game game;
static struct interval interval;
static struct runner enemies_dodge_runner, bullets_update_runner, me_shooter_runner;
static struct runner_pool pool;


#endif /* SPACESHIPGAMETYPES_H_ */