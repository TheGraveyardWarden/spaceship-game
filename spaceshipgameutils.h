#ifndef SPACESHIPGAMEUTILS_H_
#define SPACESHIPGAMEUTILS_H_

#include "spaceshipgametypes.h"
#include "spaceshipgameconfig.h"

#include "ship.h"
#include "types.h"
#include "runner_pool.h"
#include "debugger.h"
#include "interval.h"

#define FR_TO_MS(fr) (1/fr)*1000

extern void shoot_update(void* args);

u8_t is_center_valid(struct game* game, vec2_t center)
{
	u8_t i;
	struct enemy_ship* enemy;

	vec_for_each(enemy_ship, game->enemies, enemy, i, MAX_NR_ENEMIES)
	{
		if (!enemy) continue;

		if (ship_does_collide(&enemy->ship, center)) return 0;
	}

	return 1;
}

#define MAX_VALIDATION_TRIES 10

void spawn_enemy_ship(struct game* game)
{
	struct enemy_ship* enemy_ship;
	vec2_t center;
	int i;

	enemy_ship = enemy_ship_vec_get_item(game->enemies, MAX_NR_ENEMIES);
	if (!enemy_ship) return;

	i = 0;
	do {
		if (i > MAX_VALIDATION_TRIES)
		{
			enemy_ship_vec_remove(game->enemies, enemy_ship);
			return;
		}
		center = (vec2_t){gen_random_btw(1, 6), gen_random_btw(1, 6)};
		printd("in loop: %d,%d -> number of alive enemies: %d\r\n", center.x, center.y, game->nr_alive_enemies);
		i++;
	} while (!is_center_valid(game, center));

	enemy_ship_create(
		enemy_ship,
		center,
		ENEMY_MAX_HP,
		ENEMY_BASE_FIRE_RATE,
		SHIPDIRECTION_RIGHT
	);

	enemy_ship_set_runner(
		enemy_ship,
		runner_pool_add(
			&pool,
			shoot_update,
			&enemy_ship->ship.is_me,
			FR_TO_MS(enemy_ship->ship.fire_rate),
			F_TIMER
		)
	);
	interval_add_runner(&interval, enemy_ship->ship.runner);
	game->nr_alive_enemies++;
}

void game_remove_enemy_ship(struct game* game, u8_t idx)
{
	struct runner* runner;

	runner = game->enemies[idx].enemy_ship.ship.runner;

	interval_remove_runner(&interval, runner);
	runner_pool_remove(&pool, runner);
	enemy_ship_vec_remove_idx(game->enemies, idx);
	// now u can add more enemy ships because ur opponent is dead :(
	// spawn_enemy_ship(game);
}

void ship_update_bullets(struct ship* ship, struct game* game)
{
	int i, j;
	struct bullet* bullet;
	struct enemy_ship* enemy;
	struct ship* _ship;

	vec_for_each(bullet, ship->bullets, bullet, i, MAX_NR_BULLETS_PER_SHIP)
	{
		if (!bullet) continue;
		bullet_move(bullet);

		vec_for_each(enemy_ship, game->enemies, enemy, j, MAX_NR_ENEMIES)
		{
			if (!enemy) continue;

			_ship = &enemy->ship;

			if ((ship_check_hit(_ship, bullet)) == 0)
			{
				ship_remove_bullet(ship, i);
				_ship->hp--;
				if ((ship_check_dead(_ship)) == 0)
				{
					game_remove_enemy_ship(game, j);
					game->nr_alive_enemies--;
					game->nr_dead_enemies++;
					if (!game->nr_alive_enemies)
					{
						spawn_enemy_ship(game);
						spawn_enemy_ship(game);
						spawn_enemy_ship(game);
						spawn_enemy_ship(game);
					}
				}
			}
		}

		if (bullet->pos.y > 15 || bullet->pos.y < 0)
		{
			ship_remove_bullet(ship, i);
			continue;
		}
	}
}

void enemy_ship_update_bullets(struct ship* ship, struct game* game)
{
	int i;
	struct bullet* bullet;

	vec_for_each(bullet, ship->bullets, bullet, i, MAX_NR_BULLETS_PER_SHIP)
	{
		if (!bullet) continue;
		bullet_move(bullet);

		if ((ship_check_hit(&game->me, bullet)) == 0)
		{
			ship_remove_bullet(ship, i);
			game->me.hp--;
			printd("-1\r\n");
			if ((ship_check_dead(&game->me)) == 0) // we lost :(
			{
				game->status = LOST;
			}
		}

		if (bullet->pos.y > 15 || bullet->pos.y < 0)
		{
			ship_remove_bullet(ship, i);
			continue;
		}
	}
}


#endif /* SPACESHIPGAMEUTILS_H_ */