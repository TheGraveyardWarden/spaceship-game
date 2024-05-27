#ifndef VEC_H_
#define VEC_H_

#include <stdint.h>

typedef struct {
    int8_t x, y;
} vec2_t;

#define VEC2_SET(vec2, _x, _y) ({    \
    vec2.x = _x; \
    vec2.y = _y;  \
})

void vec2_copy(vec2_t* dst, const vec2_t* src, unsigned long n, unsigned long offset) {
    unsigned long i;

    for (i = offset; i < offset+n; i++) {
        dst[i].x = src[i].x;
        dst[i].y = src[i].y;
    }
}

uint8_t vec2_eq(vec2_t* this, vec2_t* other)
{
    return this->x == other->x && this->y == other->y;
}

#endif /* VEC_H_ */