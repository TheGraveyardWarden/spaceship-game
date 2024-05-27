#ifndef RANDOM_H_
#define RANDOM_H_

typedef char i8_t;
typedef unsigned char u8_t;

#define SEED 5
#define MAX 16

u8_t gen_random() {
    static long N = SEED;
    u8_t ret;

    ret = N = (N*5+7)%MAX;
    return ret;
}

i8_t gen_random_btw(i8_t x, i8_t y)
{
    return x + ((float)gen_random() / 15.0) * (y - x);
}


#endif /* RANDOM_H_ */
