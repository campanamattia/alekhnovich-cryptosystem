#ifndef BITOP_H
#define BITOP_H

#include <stdint.h>

/* Function to fetch the value of the bit at a specified position in a 64-bit integer */
int fetch_bit(uint64_t n, int pos);

/* Function to shift a 64-bit integer by a specified number of positions */
uint64_t shift_bit(uint64_t n, int pos);

/* Function to count how many bits set to 1 */
uint64_t count_ones(uint64_t n);

#endif // BITOP_H
