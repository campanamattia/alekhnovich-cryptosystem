#include "../include/api.h"

#include <stdlib.h>

#include "../include/backend.h"
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

void encrypt(const char *mex, const char *a_path, const char *y_path) {
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
    message->data = convert_to_array(mex);

    word = array_xor(message, tmp);

    if(word->data == NULL)
        return;

    write_packet(WRNNC, nnc);
    write_packet(ENCRY, word);
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

void correct(const char *path1, const char *path2, const char *path3) {
    struct arr *a, *b, *c;

    a = read_packet(path1);
    b = read_packet(path2);
    c = read_packet(path3);

    if(a == NULL || b == NULL || c == NULL)
        return;

    struct arr *res = correct_errors(a, b, c);

    write_packet(PLAIN, res);
}
