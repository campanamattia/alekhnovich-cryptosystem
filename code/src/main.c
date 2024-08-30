#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arrays.h"

//Constants
#define L 13000 //fixed
#define T 144 //fixed

//k + l <= Rn (R < 1)
#define K 1300 //euristic
#define N 16000 //euristic

//Paths
#define PUBLI "al/publi.txt"
#define PRIVA "al/priva.txt"
#define ENCRY "al/encry.txt"
#define NOISY "al/noisy.txt"
#define PLAIN "al/plain.txt"

//Command enumeration
typedef enum { GENERATE, ENCRYPT, DECRYPT, CORRECT, INVALID } Command;

//main functions
void generate_key();
void encrypt(uint64_t, const char *);
void decrypt(const char *, const char *);
void correct(const char *, const char *, const char *);

//helper functions
Command get_command(const char *);
void print_err(const char *, const char *);
int set_paths(const char *);

void write_public(struct mat *, struct mat *);
void write_private(struct mat *);

struct mat *read_public(const char *, char);
void write_message(const char *,  struct arr *);

struct mat *read_private(const char *);
void elab_message(const char *, struct arr *, struct arr *);

struct arr *read_word(FILE *);

void print_mat(char c, struct mat *m){
    printf("%c", c);
    for(int i = 0; i < m->rows; i++){
        for(int j = 0; j < real_dim(m->cols); j++)
            printf("%lu |", m->data[i][j]);
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [<args>]\n", argv[0]);
        return 1;
    }

    Command cmd = get_command(argv[1]);

    switch(cmd) {
        case GENERATE:
            generate_key();
            break;

        case ENCRYPT:
            if (argc < 5) {
                print_err(argv[0], "encrypt <message> <key_path>\n");
                return 2;
            }
            encrypt((uint64_t)argv[2], argv[3]); //TO DO: convert string to array of uint64_t
            break;

        case DECRYPT:
            if (argc < 5) {
                print_err(argv[0], "decrypt <message_path> <key_path>\n");
                return 2;
            }
            decrypt(argv[2], argv[3]);
            break;

        case CORRECT:
            if (argc < 5) {
                print_err(argv[0], "correct <input_path> <input_path> <input_path> ...\n");
                return 2;
            }
            correct(argv[2], argv[3], argv[4]);
            break;

        default:
            printf("Invalid command: %s\n", argv[1]);
            return 3;
    }

    return 0;
}

void print_err(const char* name, const char* msg) {
    fprintf(stderr, "Usage: %s %s", name, msg);
}

Command get_command(const char* command) {
    if (strcmp(command, "generate") == 0)
        return GENERATE;
    if (strcmp(command, "encrypt") == 0)
        return ENCRYPT;
    if (strcmp(command, "decrypt") == 0)
        return DECRYPT;
    if (strcmp(command, "correct") == 0)
        return CORRECT;
    return INVALID;
}

void generate_key() {
    fprintf(stdout, "generating key...\n");
    struct mat *a, *s, *e, *y;

    fprintf(stdout, "generating A...\n");
    a = rand_mat(K, N);

    fprintf(stdout, "generating S...\n");
    s = rand_mat(L, K);

    fprintf(stdout, "generating E...\n");
    e = weight_matrix(L, N, T);

    if(a == NULL ||s == NULL || e == NULL )
        return;

    fprintf(stdout, "computing Y\n");
    struct mat *tmp = matrix_mul(s, a);
    if(tmp == NULL){
        fprintf(stderr, "Unable to multiply matrices\n");
        return;
    }
    y = matrix_sum(tmp, e);

    if(y == NULL)
        return;

    write_public(a, y);
    write_private(s);
}

void write_public(struct mat *a, struct mat *y) {
    FILE *file = fopen(PUBLI, "w");
    if(file == NULL){
        fprintf(stderr, "Unable to open file %s\n", PUBLI);
        return;
    }

    fprintf(file, "Matrix A: %d x %d\n", a->rows, a->cols);
    for(int i = 0; i < a->rows; i++)
        for(int j = 0; j < real_dim(a->cols); j++)
            fprintf(file, "%016" PRIx64, a->data[i][j]);
    fprintf(file, "\n\n");

    fprintf(file, "\nMatrix Y: %d x %d\n", y->rows, y->cols);
    for(int i = 0; i < y->rows; i++)
        for(int j = 0; j < real_dim(y->cols); j++)
            fprintf(file, "%016" PRIx64, y->data[i][j]);

    fclose(file);
}

void write_private(struct mat *s) {
    FILE *file = fopen(PRIVA, "w");
    if(file == NULL){
        fprintf(stderr, "Unable to open file %s\n", PRIVA);
        return;
    }

    fprintf(file, "Matrix S: %d x %d\n", s->rows, s->cols);
    for(int i = 0; i < s->rows; i++)
        for(int j = 0; j < real_dim(s->cols); j++)
            fprintf(file, "%016" PRIx64, s->data[i][j]);

    fclose(file);
}

void encrypt(uint64_t mex, const char *key_path) {
    struct mat *a, *y;
    struct arr *nnc, *word, *tmp;

    a = read_public(key_path, 'A');
    y = read_public(key_path, 'Y');

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
    message->data = &mex;

    word = array_xor(message, tmp);

    if(word->data == NULL)
        return;

    write_message(ENCRY, concat_arrays(nnc, word));
}

struct mat *read_public(const char *path, char id){
    FILE *file = fopen(path, "r");
    if(file == NULL){
        fprintf(stderr, "Unable to open file %s\n", path);
        return NULL;
    }

    struct mat *matrix = NULL;
    char buffer[256];
    int rows, cols;

    while (fgets(buffer, sizeof(buffer), file)) {
        if ((id == 'A' && strstr(buffer, "Matrix A:")) || (id == 'Y' && strstr(buffer, "Matrix Y:"))) {
            sscanf(buffer, "Matrix %*c: %d x %d", &rows, &cols);
            matrix = calloc(1ULL, sizeof(struct mat));
            break;
        }
    }

    if (matrix == NULL) {
        fprintf(stderr, "Matrix %c not found in file\n", id);
        fclose(file);
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;

    matrix->data = calloc(rows, sizeof(uint64_t *));
    if(matrix->data == NULL)
        return NULL;

    for(int i = 0; i < rows; i++) {
        matrix->data[i] = calloc(real_dim(cols), SIZE);
        if(matrix->data[i] == NULL){
            free_mat(matrix);
            return NULL;
        }

        for(int j = 0; j < real_dim(cols); j++)
            fscanf(file, "%016" SCNu64, &matrix->data[i][j]);
    }

    fclose(file);
    return matrix;
}

void write_message(const char *output_path, struct arr *message) {
    FILE *file = fopen(output_path, "w");
    if(file == NULL){
        fprintf(stderr, "Unable to open file %s\n", output_path);
        return;
    }

    fprintf(file, "Message: %d\n", message->len);
    for(int i = 0; i < message->len; i++)
        fprintf(file, "%016" PRIx64, message->data[i]);

    fclose(file);
}

void decrypt(const char *cypher, const char *key_path) {
    struct mat *s;
    struct arr *nnc, *word, *message;

    s = read_private(key_path);
    elab_message(cypher, nnc, word);
    if(s == NULL || nnc == NULL || word == NULL){
        fprintf(stderr, "Unable to continue\n");
        return;
    }

    struct arr *key = mat_arr_mul(s, nnc);
    if(key == NULL){
        fprintf(stderr, "Unable to produce the key\n");
        return;
    }

    message = array_xor(word, key);

    write_message(NOISY, message);
}

struct mat *read_private(const char *path) {
    FILE *file = fopen(path, "r");
    if(file == NULL){
        fprintf(stderr, "Unable to open file %s\n", path);
        return NULL;
    }

    struct mat *matrix = NULL;
    char buffer[256];
    int rows, cols;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "Matrix S:")) {
            sscanf(buffer, "Matrix S: %d x %d", &rows, &cols);
            matrix = calloc(1ULL, sizeof(struct mat));
            break;
        }
    }

    if (matrix == NULL) {
        fprintf(stderr, "Matrix S not found in file\n");
        fclose(file);
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;

    matrix->data = calloc(rows, sizeof(uint64_t *));
    if(matrix->data == NULL)
        return NULL;

    for(int i = 0; i < rows; i++) {
        matrix->data[i] = calloc(real_dim(cols), SIZE);
        if(matrix->data[i] == NULL){
            free_mat(matrix);
            return NULL;
        }

        for(int j = 0; j < real_dim(cols); j++)
            fscanf(file, "%016" SCNu64, &matrix->data[i][j]);
    }

    fclose(file);
    return matrix;
}

void elab_message(const char *path, struct arr* nnc, struct arr* word){
    nnc = NULL;
    word = NULL;

    FILE *file = fopen(path, "r");
    if(file == NULL){
        fprintf(stderr, "Unable to open file %s\n", path);
        return;
    }

    struct arr *message = NULL;
    char buffer[256];
    int len;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "Message:")) {
            sscanf(buffer, "Message: %d\n", &len);
            message = calloc(1ULL, sizeof(struct arr));
            break;
        }
    }

    if (message == NULL) {
        fprintf(stderr, "Message not found in file\n");
        fclose(file);
        return;
    }

    if(len != real_dim(L) + real_dim(N)){
        fprintf(stderr, "Invalid message length\n");
        fclose(file);
        return;
    }

    message->len = len;
    message->data = calloc(len, SIZE);
    if(message->data == NULL)
        return;

    for(int i = 0; i < len; i++)
        fscanf(file, "%016" SCNu64, &message->data[i]);
    fclose(file);

    nnc = calloc(1ULL, sizeof(struct arr));
    nnc->len = real_dim(N);
    nnc->data = calloc(real_dim(N), SIZE);
    if(nnc->data == NULL)
        return;

    word = calloc(1ULL, sizeof(struct arr));
    word->len = real_dim(L);
    word->data = calloc(real_dim(L), SIZE);
    if(word->data == NULL)
        return;

    for(int i = 0; i < real_dim(N) && i < message->len; i++)
        nnc->data[i] = message->data[i];

    for(int i = 0; i < real_dim(L) && i < message->len; i++)
        word->data[i] = message->data[real_dim(N) + i];
}

void correct(const char *path1, const char *path2, const char *path3) {
    FILE *f1, *f2, *f3;

    f1 = fopen(path1, "r");
    f2 = fopen(path2, "r");
    f3 = fopen(path3, "w");

    if(f1 == NULL || f2 == NULL || f3 == NULL){
        fprintf(stderr, "Unable to open files\n");
        return;
    }

    struct arr *a, *b, *c;
    a = read_word(f1);
    b = read_word(f2);
    c = read_word(f3);

    if(a == NULL || b == NULL || c == NULL){
        fprintf(stderr, "Unable to read words\n");
        return;
    }

    if(a->len != b->len || b->len != c->len){
        fprintf(stderr, "Words have different lengths\n");
        return;
    }

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

    write_message(PLAIN, res);
}

struct arr *read_word(FILE *fp){
    if (fp == NULL) {
        fprintf(stderr, "Unable to open file\n");
        return NULL;
    }

    struct arr *word = calloc(1ULL, sizeof(struct arr));
    if(word == NULL)
        return NULL;

    char buffer[256];
    int len;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, "Message:")) {
            sscanf(buffer, "Message: %d\n", &len);
            word->len = len;
            word->data = calloc(len, SIZE);
            if(word->data == NULL)
                return NULL;

            for(int i = 0; i < real_dim(len); i++)
                fscanf(fp, "%016" SCNu64, &word->data[i]);
            return word;
        }
    }

    return NULL;
}
