#include "../include/bitop.h"

#include <stdint.h>

int fetch_bit(uint64_t n, int pos){
    return n >> pos & 1ULL;
}

uint64_t shift_bit(uint64_t n, int pos){
    return n << pos;
}

uint64_t count_ones(uint64_t n){
    uint64_t count = 0;

    for(int i = 0; i < sizeof(uint64_t); i++)
        count += fetch_bit(n, i);

    return count;
}
