#ifndef SPACESHIPGAMEUPDATERS_H_
#define SPACESHIPGAMEUPDATERS_H_

#include "spaceshipgametypes.h"

#include "ship.h"
#include "types.h"
#include "debugger.h"

#include "spaceshipgameutils.h"
#include "spaceshipgameconfig.h"
#include "generic_vector.h"

void game_on_input(char key)
{
	switch(key)
	{
		case 'w':
			ship_move(&game.me, SHIPDIRECTION_UP);
			break;
		case 's':
			ship_move(&game.me, SHIPDIRECTION_DOWN);
			break;
		case 'd':
			ship_move(&game.me, SHIPDIRECTION_RIGHT);
			break;
		case 'a':
			ship_move(&game.me, SHIPDIRECTION_LEFT);
			break;
	}
}

void enemies_dodge_update(void* args)
{
	struct enemy_ship* enemy;
	int i;

	vec_for_each(enemy_ship, game.enemies, enemy, i, MAX_NR_ENEMIES)
	{
		if (!enemy) continue;
		enemy_ship_dodge(enemy);
	}
}

void bullets_update(void* args)
{
	int i;
	struct enemy_ship* enemy;

	ship_update_bullets(&game.me, &game);

	vec_for_each(enemy_ship, game.enemies, enemy, i, MAX_NR_ENEMIES)
	{
		if (!enemy) continue;
		enemy_ship_update_bullets(&enemy->ship, &game);
	}
}

void shoot_update(void* args)
{
	u8_t* is_me;
	struct ship* ship;
	struct bullet bullet;

	is_me = (u8_t*)args;
	ship = c(is_me, struct ship, is_me);

	if (ship->hp < 1) return;

	bullet_create(
		&bullet,
		(vec2_t){
			ship->body[SHIPBODY_CENTER].x,
			ship->body[SHIPBODY_CENTER].y - (*is_me ? 1 : -1)
		},
		*is_me ? BULLETDIRECTION_UP : BULLETDIRECTION_DOWN
	);
	ship_add_bullet(ship, &bullet);
}


#endif /* SPACESHIPGAMEUPDATERS_H_ */