#ifndef ARRAYS_H
#define ARRAYS_H

#include <stdint.h>

#define SIZE (sizeof(uint64_t) * 8)

struct mat {
    int rows;
    int cols;
    uint64_t **data;
};

struct arr {
    int len;
    uint64_t *data;
};

int real_dim(int n);
void free_mat(struct mat *m);

struct mat *rand_mat(int rows, int cols);

uint64_t *weight_array(int len, int weight);
struct mat *weight_matrix(int rows, int cols, int weight);

struct mat *matrix_transpose(struct mat *m);
uint64_t bax(uint64_t *a, uint64_t *b, int len);
struct mat *matrix_mul(struct mat *a, struct mat *b);

uint64_t *array_sum(uint64_t *a, uint64_t *b, int len);
struct mat *matrix_sum(struct mat *a, struct mat *b);
struct arr *array_xor(struct arr *a, struct arr *b);

struct arr *mat_arr_mul(struct mat *mat, struct arr *arr);
struct arr *concat_arrays(struct arr *a, struct arr *b);

#endif
