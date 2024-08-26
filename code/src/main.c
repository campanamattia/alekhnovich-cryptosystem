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

#define MAX_PATH 1042

//Command enumeration
typedef enum { GENERATE, ENCRYPT, DECRYPT, CORRECT, INVALID } Command;

//main functions
void generate_key(const char *);
void encrypt(const char *, const char *, const char *);
void decrypt(const char *, const char *, const char *);
void correct(const char *, const char *);

//helper functions
Command get_command(const char *);
void print_err(const char *, const char *);
int set_paths(const char *);

void write_public(struct mat *, struct mat *);
void write_private(struct mat *);

struct mat *read_public(const char *, char);
void write_message(const char *,  struct arr *);

struct mat *read_private(const char *);

//Global variables
char public[MAX_PATH];
char private[MAX_PATH];

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [<args>]\n", argv[0]);
        return 1;
    }

    Command cmd = get_command(argv[1]);

    switch(cmd) {
        case GENERATE:
            if (argc < 3) {
                print_err(argv[0], "generate <output_key_dir>\n");
                return 2;
            }
            generate_key(argv[2]);
            break;

        case ENCRYPT:
            if (argc < 5) {
                print_err(argv[0], "encrypt <message> <key_path> <output_dir>\n");
                return 2;
            }
            encrypt(argv[2], argv[3], argv[4]);
            break;

        case DECRYPT:
            if (argc < 5) {
                print_err(argv[0], "decrypt <message> <key_path> <output_dir>\n");
                return 2;
            }
            decrypt(argv[2], argv[3], argv[4]);
            break;

        case CORRECT:
            if (argc < 5) {
                print_err(argv[0], "correct <input_path> <input_path> <input_path> ...\n");
                return 2;
            }
            correct(argv[2], argv[3]);
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

int set_paths(const char *output_dir) {
    // Usa snprintf per una copia sicura e controllata
    if (snprintf(public, sizeof(public), "%s/.public.txt", output_dir) >= sizeof(public)) {
        fprintf(stderr, "Path too long \n");
        return 0;
    }

    if (snprintf(private, sizeof(private), "%s/.private.txt", output_dir) >= sizeof(private)) {
        fprintf(stderr, "Path too long\n");
        return 0;
    }

    return 1;
}

void generate_key(const char *output_dir) {
    struct mat *a, *s, *e, *y;

    a = rand_mat(K, N);
    s = rand_mat(L, K);
    e = weight_matrix(L, N, T);

    if(a == NULL ||s == NULL || e == NULL )
        return;

    struct mat *tmp = matrix_mul(s, a);
    y = matrix_sum(tmp, e);
    free_mat(tmp);

    if(y == NULL)
        return;

    if(!set_paths(output_dir))
        return;

    write_public(a, y);
    write_private(s);
}

void write_public(struct mat *a, struct mat *y) {
    FILE *file = fopen(public, "w");
    if(file == NULL){
        fprintf(stderr, "Unable to open file %s\n", public);
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
    FILE *file = fopen(private, "w");
    if(file == NULL){
        fprintf(stderr, "Unable to open file %s\n", private);
        return;
    }

    fprintf(file, "Matrix S: %d x %d\n", s->rows, s->cols);
    for(int i = 0; i < s->rows; i++)
        for(int j = 0; j < real_dim(s->cols); j++)
            fprintf(file, "%016" PRIx64, s->data[i][j]);

    fclose(file);
}

void encrypt(const char *message, const char *key_path, const char *output_path) {
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

    nnc = compute_nonce(a, e);
    tmp = compute_nonce(y, e);
    if(nnc == NULL || tmp == NULL)
        return;

    word = calloc(1ULL, sizeof(struct arr));
    if(word == NULL)
        return;

    word->len = L;
    word->data = array_sum(tmp->data, (uint64_t *) message, word->len);

    if(word->data == NULL)
        return;

    write_message(output_path, concat_arrays(nnc, word));
}

struct mat *read_public(const char *path, char id){
    FILE *file = fopen(path, "r");
    if(file == NULL){
        fprintf(stderr, "Unable to open file %s\n", public);
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

void decrypt(const char *cypher, const char *key_path, const char *output_path) {
    struct mat *a, *s, *c, *m;

    s = read_private(key_path);

    if(a == NULL || s == NULL || c == NULL)
        return;

    struct mat *tmp = matrix_mul(c, s);
    m = matrix_sum(tmp, m);
    if(tmp == NULL)
        return;

    //write_message(output_path, m);
}
