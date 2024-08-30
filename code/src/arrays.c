#include "../include/arrays.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../include/bitop.h"
#include "../include/xoshiro.h"
#include "../include/seed.h"


int real_dim(int n) {
    return n / SIZE + (n % SIZE ? 1 : 0);
}

void free_mat(struct mat *m) {
    if (m == NULL)
        return;

    if (m->data != NULL) {
        for (int i = 0; i < m->rows; i++) {
            if (m->data[i] != NULL)
                free(m->data[i]);
        }
        free(m->data);
    }
    free(m);
}

struct mat *rand_mat(int rows, int cols) {
    if (rows == 0 || cols == 0)
        return NULL;

    struct mat *m = calloc(1, sizeof(struct mat));
    if (m == NULL)
        return NULL;

    m->rows = rows;
    m->cols = cols;

    m->data = calloc(rows, sizeof(uint64_t *));
    if (m->data == NULL) {
        free(m);
        return NULL;
    }

    init_seed();

    for (int i = 0; i < rows; i++) {
        m->data[i] = calloc(real_dim(cols), sizeof(uint64_t));
        if (m->data[i] == NULL) {
            free_mat(m);
            return NULL;
        }

        for (int j = 0; j < real_dim(cols); j++)
            m->data[i][j] = next();
    }

    return m;
}

uint64_t *weight_array(int len, int weight) {
    if (len == 0 || weight == 0)
        return NULL;

    uint64_t *array = calloc(real_dim(len), sizeof(uint64_t));
    if (array == NULL)
        return NULL;

    init_seed();

    for (int i = 0; i < weight; i++) {
        int pos;
        do {
            pos = next() % len;
        } while (array[pos / SIZE] & (1ULL << (pos % SIZE)));

        array[pos / SIZE] |= (1ULL << (pos % SIZE));
    }

    return array;
}

struct mat *weight_matrix(int rows, int cols, int weight) {
    if (rows == 0 || cols == 0 || weight == 0)
        return NULL;

    struct mat *m = calloc(1, sizeof(struct mat));
    if (m == NULL)
        return NULL;

    m->rows = rows;
    m->cols = cols;

    m->data = calloc(rows, sizeof(uint64_t *));
    if (m->data == NULL) {
        free(m);
        return NULL;
    }

    init_seed();

    for (int i = 0; i < rows; i++) {
        m->data[i] = weight_array(cols, weight);
        if (m->data[i] == NULL) {
            free_mat(m);
            return NULL;
        }
    }

    return m;
}

struct mat *matrix_transpose(struct mat *m) {
    if (m == NULL)
        return NULL;

    struct mat *t = calloc(1, sizeof(struct mat));
    if (t == NULL)
        return NULL;

    t->rows = m->cols;
    t->cols = m->rows;

    t->data = calloc(t->rows, sizeof(uint64_t *));
    if (t->data == NULL) {
        free(t);
        return NULL;
    }

    for (int i = 0; i < t->rows; i++) {
        t->data[i] = calloc(real_dim(t->cols), sizeof(uint64_t));
        if (t->data[i] == NULL) {
            free_mat(t);
            return NULL;
        }
    }

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            int bit_pos_in_block = j % SIZE;
            int block_index = j / SIZE;

            if (m->data[i][block_index] & (1ULL << bit_pos_in_block)) {
                int transpose_block_index = i / SIZE;
                int transpose_bit_pos_in_block = i % SIZE;

                t->data[j][transpose_block_index] |= (1ULL << transpose_bit_pos_in_block);
            }
        }
    }

    return t;
}

uint64_t bax(uint64_t *a, uint64_t *b, int len) {
    uint64_t res = 0;

    for (int i = 0; i < len; i++) {
        int and = a[i] & b[i];
        res |= and;
    }

    return count_ones(res);
}

struct mat *matrix_mul(struct mat *a, struct mat *b) {
    if (a == NULL || b == NULL || a->cols != b->rows)
        return NULL;

    struct mat *c = calloc(1, sizeof(struct mat));
    if (c == NULL)
        return NULL;

    c->rows = a->rows;
    c->cols = b->cols;

    c->data = calloc(c->rows, sizeof(uint64_t *));
    if (c->data == NULL) {
        free(c);
        return NULL;
    }

    for (int i = 0; i < c->rows; i++) {
        c->data[i] = calloc(real_dim(c->cols), sizeof(uint64_t));
        if (c->data[i] == NULL) {
            free_mat(c);
            return NULL;
        }
    }

    struct mat *bt = matrix_transpose(b);
    if (bt == NULL) {
        free_mat(c);
        return NULL;
    }

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < bt->rows; j++) {
            c->data[i][j / SIZE] |= shift_bit(bax(a->data[i], bt->data[j], real_dim(a->cols)), j % SIZE);
        }
    }

    free_mat(bt);
    return c;
}

uint64_t *array_sum(uint64_t *a, uint64_t *b, int len) {
    if (a == NULL || b == NULL || len == 0)
        return NULL;

    uint64_t *res = calloc(len, sizeof(uint64_t));
    if (res == NULL)
        return NULL;

    for (int i = 0; i < len; i++)
        res[i] = a[i] | b[i];

    return res;
}

struct arr *array_xor(struct arr *a, struct arr *b) {
    if (a == NULL || b == NULL || a->len != b->len)
        return NULL;

    struct arr *res = calloc(1, sizeof(struct arr));
    if (res == NULL)
        return NULL;

    res->len = a->len;
    res->data = calloc(real_dim(res->len), sizeof(uint64_t));
    if (res->data == NULL) {
        free(res);
        return NULL;
    }

    for (int i = 0; i < res->len; i++)
        res->data[i] = a->data[i] ^ b->data[i];

    return res;
}

struct mat *matrix_sum(struct mat *a, struct mat *b) {
    if (a == NULL || b == NULL || a->rows != b->rows || a->cols != b->cols)
        return NULL;

    struct mat *res = calloc(1, sizeof(struct mat));
    if (res == NULL)
        return NULL;

    res->rows = a->rows;
    res->cols = a->cols;

    res->data = calloc(res->rows, sizeof(uint64_t *));
    if (res->data == NULL) {
        free(res);
        return NULL;
    }

    for (int i = 0; i < res->rows; i++) {
        res->data[i] = array_sum(a->data[i], b->data[i], real_dim(res->cols));
        if (res->data[i] == NULL) {
            free_mat(res);
            return NULL;
        }
    }

    return res;
}

struct arr *mat_arr_mul(struct mat *m, struct arr *a) {
    if (m == NULL || a == NULL || m->cols != a->len)
        return NULL;

    struct arr *res = calloc(1, sizeof(struct arr));
    if (res == NULL)
        return NULL;

    res->len = m->rows;
    res->data = calloc(real_dim(res->len), sizeof(uint64_t));
    if (res->data == NULL) {
        free(res);
        return NULL;
    }

    for (int i = 0; i < m->rows; i++) {
        res->data[i / SIZE] |= shift_bit(bax(m->data[i], a->data, real_dim(m->cols)), i % SIZE);
    }

    return res;
}

struct arr *concat_arrays(struct arr *a, struct arr *b) {
    if (a == NULL || b == NULL)
        return NULL;

    struct arr *res = calloc(1, sizeof(struct arr));
    if (res == NULL)
        return NULL;

    int a_len = real_dim(a->len);
    int b_len = real_dim(b->len);

    res->len = a->len + b->len;
    res->data = calloc(a_len + b_len, sizeof(uint64_t));
    if (res->data == NULL) {
        free(res);
        return NULL;
    }

    for (int i = 0; i < a_len; i++)
        res->data[i] = a->data[i];

    for (int i = 0; i < b_len; i++)
        res->data[a_len + i] = b->data[i];

    return res;
}
