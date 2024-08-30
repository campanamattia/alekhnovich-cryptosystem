#include "../include/alekhnovich.h"

#include <stdlib.h>
#include <stdio.h>

#include "../include/arrays.h"

void generate_key() {
    struct mat *a, *s, *e, *y;

    a = rand_mat(K, N);
    s = rand_mat(L, K);
    e = weight_matrix(L, N, T);

    if(a == NULL ||s == NULL || e == NULL )
        return;

    struct mat *tmp = matrix_mul(s, a);
    if(tmp == NULL)
        return;

    y = matrix_sum(tmp, e);

    if(y == NULL)
        return;

    write_key(A_PUB, a);
    write_key(Y_PUB, y);
    write_key(PRIVA, s);
}

void write_key( const char *path, struct mat *m) {
    FILE *file = fopen(path, "wb");
    if (file == NULL)
        return;

    fwrite(&(m->rows), sizeof(int), 1, file);
    fwrite(&(m->cols), sizeof(int), 1, file);

    for (int i = 0; i < m->rows; i++) {
        fwrite(m->data[i], sizeof(uint64_t), real_dim(m->cols), file);
    }

    fclose(file);
}

uint64_t *convert_to_array(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file)
        return NULL;


    uint64_t *array = calloc(real_dim(L), sizeof(uint64_t));
    if (array == NULL) {
        fclose(file);
        return NULL;
    }

    char c;
    size_t index = 0;
    for(int i = 0; i < L && fread(&c, 1, 1, file) == 1; i++){
        if(c == '1')
            array[i / SIZE] |= ((uint64_t)1 << (i % SIZE));
    }

    fclose(file);
    return array;
}

void encrypt(uint64_t *mex, const char *a_path, const char *y_path) {
    if(mex == NULL)
        return;

    struct mat *a, *y;
    struct arr *nnc, *word, *tmp;

    a = read_key(a_path);

    y = read_key(y_path);

    if(a == NULL || y == NULL)
        return;

    struct arr *e = calloc(1ULL, sizeof(struct mat));
    if(e == NULL)
        return;

    e->len = N;

    e->data = weight_array(e->len, T);
    if(e->data == NULL)
        return;

    nnc = mat_arr_mul(a, e);
    tmp = mat_arr_mul(y, e);
    if(nnc == NULL || tmp == NULL)
        return;

    word = calloc(1ULL, sizeof(struct arr));
    if(word == NULL)
        return;

    struct arr *message = calloc(1ULL, sizeof(struct arr *));
    if(message == NULL)
        return;

    message->len = L;
    message->data = mex;

    word = array_xor(message, tmp);

    if(word->data == NULL)
        return;

    write_packet(WRNNC, nnc);
    write_packet(ENCRY, word);
}

struct mat *read_key(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL)
        return NULL;

    struct mat *matrix = calloc(1ULL, sizeof(struct mat));
    if (matrix == NULL)
        return NULL;

    fread(&(matrix->rows), sizeof(int), 1, file);
    fread(&(matrix->cols), sizeof(int), 1, file);

    matrix->data = calloc(matrix->rows, sizeof(uint64_t *));
    if (matrix->data == NULL)
        return NULL;

    for (int i = 0; i < matrix->rows; i++) {
        matrix->data[i] = calloc(real_dim(matrix->cols), sizeof(uint64_t));
        if (matrix->data[i] == NULL)
            return NULL;

        fread(matrix->data[i], sizeof(uint64_t), real_dim(matrix->cols), file);
    }

    fclose(file);
    return matrix;
}


void write_packet(const char *output_path, struct arr *message) {
    FILE *file = fopen(output_path, "w");
    if(file == NULL)
        return;

    if(message == NULL || message->data == NULL)
        return;

    for(int i = 0; i < real_dim(message->len); i++);

    fclose(file);
}

void decrypt(const char *fnnc, const char *fword, const char *key_path) {
    struct mat *s;
    struct arr *nnc, *word, *message;

    s = read_key(key_path);

    nnc = read_packet(fnnc);
    word = read_packet(fword);

    if(s == NULL || nnc == NULL || word == NULL)
        return;

    struct arr *key = mat_arr_mul(s, nnc);
    if(key == NULL)
        return;

    message = array_xor(word, key);

    write_packet(NOISY, message);
}

struct arr *read_packet(const char *path){

    FILE *file = fopen(path, "r");
    if(file == NULL)
        return NULL;

    return NULL;
}

void correct(const char *path1, const char *path2, const char *path3) {
    FILE *f1, *f2, *f3;

    f1 = fopen(path1, "r");
    f2 = fopen(path2, "r");
    f3 = fopen(path3, "w");

    if(f1 == NULL || f2 == NULL || f3 == NULL)
        return;

    struct arr *a, *b, *c;
    a = read_word(f1);
    b = read_word(f2);
    c = read_word(f3);

    if(a == NULL || b == NULL || c == NULL)
        return;

    if(a->len != b->len || b->len != c->len)
        return;

    struct arr *res = calloc(1ULL, sizeof(struct arr));
    if(res == NULL)
        return;

    res->len = a->len;
    res->data = calloc(a->len, SIZE);
    if(res->data == NULL)
        return;

    for(int i = 0; i < a->len; i++){
        res->data[i] = (a->data[i] & b->data[i]) | (b->data[i] & c->data[i]) | (c->data[i] ^ a->data[i]);
    }

    write_packet(PLAIN, res);
}

struct arr *read_word(FILE *fp){
    if (fp == NULL) {
        fprintf(stderr, "Unable to open file\n");
        return NULL;
    }

    return NULL;
}
