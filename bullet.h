#ifndef BULLET_H_
#define BULLET_H_

#include "vec.h"
#include "types.h"
#include "generic_vector.h"

enum bullet_direction
{
	BULLETDIRECTION_UP = 0,
	BULLETDIRECTION_DOWN
};

struct bullet
{
	vec2_t pos;
	enum bullet_direction direction;
};

struct_vec_item(bullet);
DEFINE_VEC_METHODS(bullet);

void bullet_create(
	struct bullet* bullet,
	vec2_t pos,
	enum bullet_direction direction
)
{
	VEC2_SET(bullet->pos, pos.x, pos.y);
	bullet->direction = direction;
}

void bullet_move(struct bullet* bullet)
{
	switch(bullet->direction)
	{
		case BULLETDIRECTION_DOWN:
			bullet->pos.y++;
			break;
		case BULLETDIRECTION_UP:
			bullet->pos.y--;
			break;
	}
}


#endif /* BULLET_H_ */