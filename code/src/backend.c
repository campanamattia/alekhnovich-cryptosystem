#include "../include/backend.h"

#include <stdlib.h>
#include <stdio.h>

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
    FILE *file = fopen(output_path, "wb");
    if(file == NULL)
        return;

    if(message == NULL || message->data == NULL)
        return;

    fwrite(&(message->len), sizeof(int), 1, file);

    for(int i = 0; i < real_dim(message->len); i++)
        fwrite(&(message->data[i]), sizeof(uint64_t), 1, file);

    fclose(file);
}

struct arr *read_packet(const char *path){
    FILE *file = fopen(path, "rb");
    if(file == NULL)
        return NULL;

    struct arr *packet = calloc(1ULL, sizeof(struct arr));
    if(packet == NULL)
        return NULL;

    fread(&(packet->len), sizeof(int), 1, file);
    packet->data = calloc(real_dim(packet->len), sizeof(uint64_t));
    if(packet->data == NULL)
        return NULL;

    for(int i = 0; i < real_dim(packet->len); i++)
        fread(&(packet->data[i]), sizeof(uint64_t), 1, file);

    fclose(file);
    return packet;
}

struct arr *correct_errors(struct arr *a, struct arr *b, struct arr *c){
    if(a->len != b->len || b->len != c->len)
        return NULL;

    struct arr *res = calloc(1ULL, sizeof(struct arr));
    if(res == NULL)
        return NULL;

    res->len = a->len;
    res->data = calloc(a->len, SIZE);
    if(res->data == NULL)
        return NULL;

    for(int i = 0; i < a->len; i++){
        res->data[i] = (a->data[i] & b->data[i]) | (b->data[i] & c->data[i]) | (c->data[i] ^ a->data[i]);
    }

    return res;
}
