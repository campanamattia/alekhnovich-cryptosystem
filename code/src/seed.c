#include "../include/seed.h"

#include <randombytes.h>
#include "../include/xoshiro.h"

#define SEED 8

void reset_seed() {
    s[0] = 0;
    s[1] = 0;
    s[2] = 0;
    s[3] = 0;
}

void init_seed(){

    reset_seed();

    for(int i = 0; i < 4; i++){
        unsigned char seed[SEED];
        randombytes(seed, SEED);

        for(int j = 0; j < SEED; j++){
            s[i] = (s[i] << 8) | seed[j];
        }
    }
}
