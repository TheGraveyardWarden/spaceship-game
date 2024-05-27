#ifndef _TYPES_H_
#define _TYPES_H_

#include <stddef.h>

#define offset_of(type, member) (size_t)(&((type*)0)->member)
#define container_of(ptr, type, member) (type*)((char*)ptr - offset_of(type, member))

typedef char i8_t;
typedef unsigned char u8_t;

typedef short i16_t;
typedef unsigned short u16_t;

typedef long i32_t;
typedef unsigned long u32_t;

#endif /* _TYPES_H_ */