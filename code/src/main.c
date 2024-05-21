#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
//#include <randombytes.h>

#include "../lib/xoshiro.h"
#include "../lib/bitop.h"
#include "../lib/seed.h"
#include "../lib/matrix.h"

//#define SIZE sizeof(uint64_t)
#define L 1300
#define T 144
#define K (1 << 0)
#define N (1 << 11)

enum flag{
    GENERATE,
    ENCRYPT,
    DECRYPT,
    CORRECT
};

struct key{
    int rows, cols;
    uint64_t **data;
};

//for function prototypes
enum flag get_command(char *);
char *concat(const char *, const char *);
void write_key(FILE *, struct key *);
struct key *read_key(FILE *);

void generate_keys(char *);
void encrypt_message(char *, char *, char *);
void decrypt_message(char *, char *, char *);
void correct_message(char *, char *, char *);

uint64_t **compute_public(struct key *, struct key *, struct key *);

//for global variables
//
int main(int argc, char *argv[]) {
    if (argc < 1){
        printf("Usage: <flag> <input> <output>\n");
        return 1;
    }

    switch(get_command(argv[1])){
        case GENERATE:
            generate_keys(argv[2]);
            break;
        case ENCRYPT:
            encrypt_message(argv[2], argv[3], argv[4]);
            break;
        case DECRYPT:
            decrypt_message(argv[2], argv[3], argv[4]);
            break;
        case CORRECT:
            correct_message(argv[2], argv[3], argv[4]);
            break;
        default:
            printf("Invalid command\n");
            return 1;
    } 

    return 0;
}

enum flag get_command(char *command){
    if(strcmp(command, "generate") == 0)
        return GENERATE;

    if(strcmp(command, "encrypt") == 0)
        return ENCRYPT;
    
    if(strcmp(command, "decrypt") == 0)
        return DECRYPT;
    
    if(strcmp(command, "correct") == 0)
        return CORRECT;
    
    return -1;
}

char *concat(const char *s1, const char *s2){
    char *result = calloc(1, strlen(s1) + strlen(s2) + 1);
    if(result == NULL){
        printf("Error allocating memory\n");
        return NULL;
    }

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

void write_key(FILE *file, struct key *key){
    fwrite(key->rows, sizeof(int), 1, file);
    fwrite(key->cols, sizeof(int), 1, file);
    fwrite(key->data, sizeof(uint64_t), key->rows * key->cols, file);
    fflush(stdout);
}

struct key *read_key(FILE *file){
    struct key *key = calloc(1, sizeof(struct key));

    if(key == NULL){
        printf("Error allocating memory\n");
        return NULL;
    }

    fread(key->rows, sizeof(int), 1, file);
    fread(key->cols, sizeof(int), 1, file);

    key->data = (uint64_t **) calloc(key->rows, sizeof(uint64_t *));

    if(key->data == NULL){
        printf("Error allocating memory\n");
        return NULL;
    }

    fwrite(key->data, sizeof(uint64_t), key->rows * key->cols, file);

    return key;
}

void generate_keys(char *output){
    if(output == NULL){
        printf("Output path not provided\n");
        return;
    }

    char *path;
    if (output[strlen(output) - 1] != '/') {
        path = concat(output, "/");
    } else {
        path = output;
    }
    
    FILE *private = fopen(concat(path, "key.sec"), "wb");
    FILE *public = fopen(concat(path, "key.pub"), "wb");   

    if(private == NULL || public == NULL){
        printf("Error opening file\n");
        return;
    }

    struct key *A = calloc(1, sizeof(struct key));
    struct key *S = calloc(1, sizeof(struct key));
    struct key *E = calloc(1, sizeof(struct key));
    struct key *Y = calloc(1, sizeof(struct key));

    if(A == NULL || S == NULL || E == NULL || Y == NULL){
        printf("Error allocating memory\n");
        return;
    }

    A->rows = K;
    A->cols = N;
    A->data = random_matrix(A->rows, A->cols);
    if(A->data == NULL)
        return;

    S->rows = L;
    S->cols = K;
    S->data = random_matrix(S->rows, S->cols);
    if(S->data == NULL)
        return;

    E->rows = L;
    E->cols = N;
    E->data = weighted_matrix(E->rows, E->cols, T);
    if(E->data == NULL)
        return;

    Y->rows = L;
    Y->cols = N;
    Y->data = compute_public(S, A, E);
    if(Y->data == NULL)
        return;

    write_key(public, Y);
    write_key(public, A);
    write_key(private, S);

    fclose(private);
    fclose(public);
}

uint64_t **compute_public(struct key *S, struct key *A, struct key *E){
    uint64_t **Y = matrix_product(S->data, A->data, S->rows, S->cols, A->rows, A->cols);

    if(Y == NULL){
        printf("Error computing public key\n");
        return NULL;
    }

    Y = matrix_sum(Y, E->data, E->rows, E->cols);

    return Y;
}

void encrypt_message(char *file, char *key, char *output){

}