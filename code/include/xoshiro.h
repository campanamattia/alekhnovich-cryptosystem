#ifndef XOSHIRO256_H
#define XOSHIRO256_H

#include <stdint.h>

extern uint64_t s[4];

/* Function to rotate bits left */
static inline uint64_t rotl(const uint64_t x, int k);

/* Advances the state of the generator and returns the next random number */
uint64_t next(void);

/* Jump function for the generator to generate non-overlapping subsequences */
void jump(void);

/* Long jump function for the generator for distributed computations */
void long_jump(void);

#endif // XOSHIRO256_H
