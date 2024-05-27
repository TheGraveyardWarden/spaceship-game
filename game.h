#ifndef GAME_H_
#define GAME_H_

#include "vec.h"
#include "renderer.h"
#include "interval.h"
#include "random.h"

typedef char i8_t;
typedef unsigned char u8_t;

#define MAX_SNAKE_LEN 20
#define MIN_SNAKE_LEN 3

#define INIT_DIRECTION RIGHT

enum snake_direction {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};

enum game_status {
    RUNNING,
    LOST
};

struct game {
    vec2_t snake[MAX_SNAKE_LEN];
    u8_t snake_len;
    enum snake_direction direction;
    vec2_t feed;
    enum game_status status;
};

#define DEFINE_GAME(name) struct game name

void game_init(struct game* game)
{
    int i;
    
    for (i = 0; i < MIN_SNAKE_LEN; i++) {
        VEC2_SET(game->snake[i], i, 0);
    }
    
    game->snake_len = MIN_SNAKE_LEN;
    game->direction = INIT_DIRECTION;

    VEC2_SET(game->feed, 10, 7);

    game->status = RUNNING;
    
    renderer_init();
}

void game_render(struct game* game)
{
    renderer_clear_data();
    int i;
    for (i = 0; i < game->snake_len; i++)
    {
        renderer_set_data(game->snake[i].x, game->snake[i].y, ON);
    }
    renderer_set_data(game->feed.x, game->feed.y, ON);
    renderer_draw();
}

inline u8_t game_is_running(struct game* game) {
    return game->status == RUNNING;
}

void game_lost(struct game* game)
{
    stop_interval();
    renderer_clear_data();
    game->status = LOST;
}

void set_random_feed(struct game* game)
{
    int i;
    
    VEC2_SET(game->feed, gen_random(), gen_random());
    for (i = 0; i < game->snake_len; i++)
    {
        if (vec2_eq(&game->feed, &game->snake[i]))
            return set_random_feed(game);
    }
}

void game_update(void* arg) {
    struct game* game;
    vec2_t next_pos;
    vec2_t snake_head_pos;
    int i;

    game = (struct game*)arg;

    VEC2_SET(snake_head_pos, game->snake[game->snake_len - 1].x, game->snake[game->snake_len - 1].y);

    switch (game->direction) {
        case UP:
            if (snake_head_pos.y == 0)
                VEC2_SET(next_pos, snake_head_pos.x, 15);
            else
                VEC2_SET(next_pos, snake_head_pos.x, snake_head_pos.y-1);
            break;
        case DOWN:
            if (snake_head_pos.y == 15)
                VEC2_SET(next_pos, snake_head_pos.x, 0);
            else
                VEC2_SET(next_pos, snake_head_pos.x, snake_head_pos.y+1);
            break;
        case RIGHT:
            if (snake_head_pos.x == 15)
                VEC2_SET(next_pos, 0, snake_head_pos.y);
            else
                VEC2_SET(next_pos, snake_head_pos.x + 1, snake_head_pos.y);
            break;
        case LEFT:
            if (snake_head_pos.x == 0)
                VEC2_SET(next_pos, 15, snake_head_pos.y);
            else
                VEC2_SET(next_pos, snake_head_pos.x - 1, snake_head_pos.y);
            break;
    }

    if (vec2_eq(&next_pos, &game->feed) && game->snake_len == MAX_SNAKE_LEN) {
        set_random_feed(game);
        goto next;
    } else if (vec2_eq(&next_pos, &game->feed)) {
        game->snake_len++;
        set_random_feed(game);
    } else {
    next:
        for (i = 0; i < game->snake_len - 1; i++) {
            game->snake[i] = game->snake[i+1];

            if (vec2_eq(&game->snake[i], &next_pos)) {
                    game_lost(game);
            }
        }
    }
    
    VEC2_SET(game->snake[game->snake_len - 1], next_pos.x, next_pos.y);
}

#endif /* GAME_H_ */
