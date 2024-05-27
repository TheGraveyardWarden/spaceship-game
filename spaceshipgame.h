#ifndef SPACESHIPGAME_H_
#define SPACESHIPGAME_H_

#include "config.h"
#include "spaceshipgameconfig.h"

#include "vec.h"

#define MONITOR_LAYOUT_2X1
#include "renderer.h"

#include "spaceshipgametypes.h"

#include "types.h"
#include "random.h"
#include "error.h"

#include "timer.h"
#include "interval.h"
#include "keyboard.h"


#include "ship.h"
#include "spaceshipgameupdaters.h"
#include "spaceshipgameutils.h"

#include "runner_pool.h"

DEFINE_INTERVAL_ISR(1, interval)

void game_on_input(char key);
void enemies_dodge_update(void* args);
void bullets_update(void* args);
void shoot_update(void* args);

ISR(USART_RXC_vect)
{
	game_on_input(UDR);
}

void game_init(void)
{
	int i;

	renderer_init();

	keyboard_init(F_CPU, 9600);

	runner_pool_init(&pool);
	INTERVAL_INIT(1, interval);
	runner_init(&enemies_dodge_runner, enemies_dodge_update, NULL, ENEMIES_DODGE_UPDATE_TIMEOUT, F_TIMER);
	runner_init(&bullets_update_runner, bullets_update, NULL, BULLETS_UPDATE_TIMEOUT, F_TIMER);

	sei();

	interval_add_runner(&interval, &enemies_dodge_runner);
	interval_add_runner(&interval, &bullets_update_runner);

	INTERVAL_RUN(1);

	ship_create(&game.me, PLAYER_INIT_POS, PLAYER_MAX_HP, PLAYER_BASE_FIRE_RATE, 0);
	runner_init(&me_shooter_runner, shoot_update, &game.me.is_me, FR_TO_MS(game.me.fire_rate), F_TIMER);
	ship_set_runner(&game.me, &me_shooter_runner);
	interval_add_runner(&interval, &me_shooter_runner);

	enemy_ship_vec_init(game.enemies, MAX_NR_ENEMIES);

	for (i = 0; i < INITIAL_NR_ENEMIES; i++)
		spawn_enemy_ship(&game);

	game.nr_dead_enemies = 0;

	game.status = RUNNING;
}

void game_render(void)
{
	int i;
	struct enemy_ship* enemy;
	renderer_clear_data();
	ship_draw(&game.me);
	vec_for_each(enemy_ship, game.enemies, enemy, i, MAX_NR_ENEMIES)
	{
		if (!enemy) continue;
		ship_draw(&enemy->ship);
	}
	renderer_draw();
}

void game_loop(void)
{
	while (game.status == RUNNING)
	{
		game_render();
	}
}




#endif /* SPACESHIPGAME_H_ */
