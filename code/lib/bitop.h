#include <stdint.h>

/*Returns the value of the k-th bit of elem
*/
int fetch_bit(uint64_t elem, int k){
    return (elem >> k) & 1;
}

/*Shift the bit to the k-th position
*/
uint64_t shift_bit(int bit, int k){
    return (uint64_t) bit << k;
}

/*Returns the number of 1s in elem
*/
int count_ones(uint64_t elem){
    int res = 0;

    for(int i = 0; i < sizeof(uint64_t); i++){
        res += fetch_bit(elem, i);
    }

    return res;
}

/*Returns the result of the bitwise XOR after and AND between a and b
*/
int bax(uint64_t *a, uint64_t *b, int l){
    int res = 0;

    for(int i = 0; i < l; i++){
        int and = a[i] & b[i];
        res += count_ones(and);
    }

    return res % 2;
}