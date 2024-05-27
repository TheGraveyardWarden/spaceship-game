#ifndef SHIP_H_
#define SHIP_H_

#include "spaceshipgameconfig.h"
#include "vec.h"
#include "types.h"
#include "bullet.h"
#include "renderer.h"
#include "runner.h"
#include "math.h"

enum ship_direction
{
	SHIPDIRECTION_UP = 0,
	SHIPDIRECTION_RIGHT,
	SHIPDIRECTION_LEFT,
	SHIPDIRECTION_DOWN
};

enum ship_direction ship_direction_opposite(enum ship_direction direction)
{
	switch(direction)
	{
		case SHIPDIRECTION_UP: return SHIPDIRECTION_DOWN;
		case SHIPDIRECTION_DOWN: return SHIPDIRECTION_UP;
		case SHIPDIRECTION_LEFT: return SHIPDIRECTION_RIGHT;
		case SHIPDIRECTION_RIGHT: return SHIPDIRECTION_LEFT;
	}
	return SHIPDIRECTION_DOWN;
}

enum ship_body
{
	SHIPBODY_CENTER = 0,
	SHIPBODY_LEFT,
	SHIPBODY_RIGHT,
	SHIPBODY_TOP
};

struct ship {
	vec2_t body[SHIP_BODY_SIZE];
	u8_t hp;
	float fire_rate;

	vec(bullet, bullets, MAX_NR_BULLETS_PER_SHIP);

	struct runner* runner;
	u8_t is_me;
};

void ship_create(
	struct ship* ship,
	vec2_t center,
	u8_t hp,
	float fire_rate,
	u8_t is_enemy
)
{
	VEC2_SET(ship->body[SHIPBODY_CENTER], center.x, center.y);
	VEC2_SET(ship->body[SHIPBODY_LEFT], center.x - 1, center.y);
	VEC2_SET(ship->body[SHIPBODY_RIGHT], center.x + 1, center.y);
	VEC2_SET(ship->body[SHIPBODY_TOP], center.x, is_enemy ? center.y + 1 : center.y - 1);

	ship->hp = hp;
	ship->fire_rate = fire_rate;

	bullet_vec_init(ship->bullets, MAX_NR_BULLETS_PER_SHIP);

	ship->runner = 0;
	ship->is_me = !is_enemy;
}

inline void ship_set_runner(struct ship* ship, struct runner* runner) { ship->runner = runner; }

inline void ship_add_bullet(struct ship* ship, struct bullet* bullet)
{
	bullet_vec_add(ship->bullets, bullet, MAX_NR_BULLETS_PER_SHIP);
}

inline void ship_remove_bullet(struct ship* ship, u8_t idx)
{
	bullet_vec_remove_idx(ship->bullets, idx);
}

i8_t ship_check_hit(struct ship* victim, struct bullet* killer)
{
	// if we want headshot logic it should be here ^^
	int i;

	for (i = 0; i < SHIP_BODY_SIZE; i++)
		if (vec2_eq(&victim->body[i], &killer->pos))
			return 0;

	return 1;
}

inline i8_t ship_check_dead(struct ship* ship) { return ship->hp != 0; }

u8_t ship_does_collide(struct ship* ship, vec2_t another)
{
	if (
		m_abs(ship->body[SHIPBODY_CENTER].y - another.y) < 2 &&
		m_abs(ship->body[SHIPBODY_CENTER].x - another.x) < 3
	) return 1;

	return 0;
}

#define __ship_move_up 			y--
#define __ship_move_right		x++
#define __ship_move_left		x--
#define __ship_move_down		y++

#define _ship_move(ship, d)				\
	ship->body[SHIPBODY_CENTER].d;		\
	ship->body[SHIPBODY_LEFT].d;		\
	ship->body[SHIPBODY_RIGHT].d;		\
	ship->body[SHIPBODY_TOP].d

#define _ship_move_up(ship) _ship_move(ship, __ship_move_up)
#define _ship_move_right(ship) _ship_move(ship, __ship_move_right)
#define _ship_move_left(ship) _ship_move(ship, __ship_move_left)
#define _ship_move_down(ship) _ship_move(ship, __ship_move_down)

#define _ship_can_move_up(ship) (ship->body[SHIPBODY_TOP].y != 0)
#define _ship_can_move_right(ship) (ship->body[SHIPBODY_RIGHT].x != 7)
#define _ship_can_move_left(ship) (ship->body[SHIPBODY_LEFT].x != 0)
#define _ship_can_move_down(ship) (ship->body[SHIPBODY_CENTER].y != 15)

i8_t ship_can_move(struct ship* ship, enum ship_direction direction)
{
	switch(direction)
	{
		case SHIPDIRECTION_UP:
			return _ship_can_move_up(ship);
		case SHIPDIRECTION_RIGHT:
			return _ship_can_move_right(ship);
		case SHIPDIRECTION_LEFT:
			return _ship_can_move_left(ship);
		case SHIPDIRECTION_DOWN:
			return _ship_can_move_down(ship);
		default:
			return -1;
	}
}

void ship_move(struct ship* ship, enum ship_direction direction)
{
	switch(direction)
	{
		case SHIPDIRECTION_UP:
			if (!_ship_can_move_up(ship)) return;
			_ship_move_up(ship);
			break;
		case SHIPDIRECTION_RIGHT:
			if (!_ship_can_move_right(ship)) return;
			_ship_move_right(ship);
			break;
		case SHIPDIRECTION_LEFT:
			if (!_ship_can_move_left(ship)) return;
			_ship_move_left(ship);
			break;
		case SHIPDIRECTION_DOWN:
			if (!_ship_can_move_down(ship)) return;
			_ship_move_down(ship);
			break;
	}
}

void ship_draw(struct ship* ship)
{
	int i;
	struct bullet* bullet;

	renderer_set_data_arr(ship->body, SHIP_BODY_SIZE);

	vec_for_each(bullet, ship->bullets, bullet, i, MAX_NR_BULLETS_PER_SHIP)
	{
		if (!bullet) continue;
		renderer_set_data(bullet->pos.x, bullet->pos.y, ON);
	}
}

struct enemy_ship
{
	struct ship ship;
	enum ship_direction direction;
};

struct_vec_item(enemy_ship);
DEFINE_VEC_METHODS(enemy_ship);

inline void enemy_ship_create(
	struct enemy_ship* enemy_ship,
	vec2_t center,
	u8_t hp,
	float fire_rate,
	enum ship_direction direction
)
{
	ship_create(&(enemy_ship->ship), center, hp, fire_rate, 1);
	enemy_ship->direction = direction;
}

inline void enemy_ship_set_runner(struct enemy_ship* enemy_ship, struct runner* runner)
{
	ship_set_runner(&enemy_ship->ship, runner);
}

void enemy_ship_dodge(struct enemy_ship* enemy_ship)
{
	if (!ship_can_move(&enemy_ship->ship, enemy_ship->direction))
		enemy_ship->direction = ship_direction_opposite(enemy_ship->direction);
	ship_move(&enemy_ship->ship, enemy_ship->direction);
}


#endif /* SHIP_H_ */