#include "../include/test.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "../include/backend.h"

#define TEST1 "target/test1.bin"
#define TEST2 "target/test2.bin"
#define TEST3 "target/test3.bin"

void shortcut( );

void handle_error(const char *);
void free_resources(struct mat *, struct mat *, struct mat *, struct mat *, struct arr *, struct arr *, struct arr *);

struct arr* generate_random_message(int );
void generate_matrices(struct mat **, struct mat **, struct mat **);
void check_key_reading(struct mat *, struct mat *, struct mat *);
void encryption_decryption_process(struct mat *, struct mat *, struct mat *, struct arr *);
void encrypt_decrypt_shortcut(struct mat *, struct mat *, struct mat *, struct arr *, const char *);
void print_hamming_distance(struct arr *, struct arr *);

int check_mat(struct mat *, struct mat *);
int check_arr(struct arr *, struct arr *);
int delta_arr(struct arr *, struct arr *);

void test(int mod) {
    if(mod){
        shortcut();
        return;
    }
    fprintf(stdout, "Starting the testing process...\n");

    struct mat *aw = NULL, *sw = NULL, *ew = NULL, *yw = NULL;

    generate_matrices(&aw, &sw, &ew);

    fprintf(stdout, "Computing matrix y...\n");
    struct mat *tmp_mat = matrix_mul(sw, aw);
    if (tmp_mat == NULL) {
        handle_error("Failed to multiply matrices.");
        free_resources(aw, sw, ew, NULL, NULL, NULL, NULL);
        return;
    }

    yw = matrix_sum(tmp_mat, ew);
    free_mat(tmp_mat);

    write_key(A_PUB, aw);
    write_key(Y_PUB, yw);
    write_key(PRIVA, sw);

    fprintf(stdout, "Matrices generated successfully.\n\n");

    check_key_reading(aw, yw, sw);

    fprintf(stdout, "Starting encryption and decryption check...\n");
    struct arr *msg = generate_random_message(L);
    encryption_decryption_process(aw, yw, sw, msg);

    free_resources(aw, sw, ew, yw, msg, NULL, NULL);
}

// Error handling function
void handle_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

// Resource management function
void free_resources(struct mat *m1, struct mat *m2, struct mat *m3, struct mat *m4, struct arr *a1, struct arr *a2, struct arr *a3) {
    if (m1) free_mat(m1);
    if (m2) free_mat(m2);
    if (m3) free_mat(m3);
    if (m4) free_mat(m4);
    if (a1) free(a1->data);
    if (a1) free(a1);
    if (a2) free(a2->data);
    if (a2) free(a2);
    if (a3) free(a3->data);
    if (a3) free(a3);
}

// Generates random message array
struct arr* generate_random_message(int length) {
    uint64_t *m = calloc(real_dim(length), sizeof(uint64_t));
    if (m == NULL) {
        handle_error("Memory allocation failed for message.");
        return NULL;
    }

    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        if (rand() % 2) {
            m[i / SIZE] |= 1 << (i % SIZE);
        }
    }

    struct arr *msg = malloc(sizeof(struct arr));
    if (msg == NULL) {
        handle_error("Memory allocation failed for msg struct.");
        free(m);
        return NULL;
    }
    msg->len = length;
    msg->data = m;
    write_packet(WRGEN, msg);

    return msg;
}

void generate_matrices(struct mat **aw, struct mat **sw, struct mat **ew) {
    fprintf(stdout, "Generating matrices...\n");

    *aw = rand_mat(K, N);
    *sw = rand_mat(L, K);
    *ew = weight_matrix(L, N, T);

    if (*aw == NULL || *sw == NULL || *ew == NULL) {
        handle_error("Failed to generate matrices.");
        free_resources(*aw, *sw, *ew, NULL, NULL, NULL, NULL);
        exit(EXIT_FAILURE);
    }
}

void check_key_reading(struct mat *aw, struct mat *yw, struct mat *sw) {
    fprintf(stdout, "Checking the reading of public and private keys...\n");

    struct mat *ar = read_key(A_PUB);
    struct mat *yr = read_key(Y_PUB);
    struct mat *sr = read_key(PRIVA);

    if (ar == NULL || yr == NULL || sr == NULL) {
        handle_error("Failed to read keys.");
        free_resources(aw, sw, NULL, yw, NULL, NULL, NULL);
        exit(EXIT_FAILURE);
    }

    if (!check_mat(aw, ar) || !check_mat(yw, yr) || !check_mat(sw, sr)) {
        handle_error("Keys do not match expected values.");
        free_resources(aw, sw, NULL, yw, NULL, NULL, NULL);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Keys read correctly.\n\n");

    free_resources(ar, yr, sr, NULL, NULL, NULL, NULL);
}

void encryption_decryption_process(struct mat *ar, struct mat *yr, struct mat *sr, struct arr *msg) {
    struct arr *e = calloc(1ULL, sizeof(struct arr));
    if (e == NULL) {
        handle_error("Memory allocation failed for e struct.");
        return;
    }
    e->len = N;
    e->data = weight_array(N, T);
    if (e->data == NULL) {
        handle_error("Memory allocation failed for e data.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, NULL);
        return;
    }

    struct arr *w_nnc = mat_arr_mul(ar, e);
    if (w_nnc == NULL) {
        handle_error("Failed to multiply matrix with array.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, NULL);
        return;
    }

    struct arr *tmp_arr = mat_arr_mul(yr, e);
    if (tmp_arr == NULL) {
        handle_error("Failed to multiply matrix with array.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, w_nnc);
        return;
    }

    struct arr *w_word = array_xor(tmp_arr, msg);
    if (w_word == NULL) {
        handle_error("Failed to XOR arrays.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    write_packet(WRNNC, w_nnc);
    write_packet(ENCRY, w_word);

    struct arr *r_nnc = read_packet(WRNNC);
    struct arr *r_word = read_packet(ENCRY);

    if (r_nnc == NULL || r_word == NULL) {
        handle_error("Failed to read encrypted packets.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    if (!check_arr(w_nnc, r_nnc) || !check_arr(w_word, r_word)) {
        handle_error("Packet transmission failed.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    fprintf(stdout, "Packets transmitted and verified correctly.\n\n");

    struct arr *dec_key = mat_arr_mul(sr, r_nnc);
    if (dec_key == NULL) {
        handle_error("Failed to decrypt key.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    struct arr *dec_msg = array_xor(r_word, dec_key);
    if (dec_msg == NULL) {
        handle_error("Failed to decrypt message.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    write_packet(NOISY, dec_msg);

    struct arr *r_noisy = read_packet(NOISY);
    if (r_noisy == NULL) {
        handle_error("Failed to read decrypted message.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    if (!check_arr(dec_msg, r_noisy)) {
        handle_error("Decrypted message does not match.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    fprintf(stdout, "Decryption process completed successfully.\n\n");

    print_hamming_distance(msg, dec_msg);

    free_resources(NULL, NULL, NULL, NULL, e, dec_key, dec_msg);
}

void print_hamming_distance(struct arr *msg, struct arr *dec_msg) {
    int distance = delta_arr(msg, dec_msg);
    fprintf(stdout, "Hamming distance from original message: %d\n\n", distance);
}

int check_mat(struct mat *w, struct mat *r){
    if(w->cols != r->cols || w->rows != r->rows)
        return 0;
    for(int i = 0; i < r->rows; i++)
        for(int j = 0; j < real_dim(r->cols); j++)
            if(r->data[i][j] != w->data[i][j])
                return 0;

    return 1;
}

int check_arr(struct arr *w, struct arr *r){
    if(w->len != r->len)
        return 0;
    for(int i = 0; i < real_dim(r->len); i++)
        if(r->data[i] != w->data[i])
            return 0;

    return 1;
}

int delta_arr(struct arr *o, struct arr *e) {
    if (o->len != e->len)
        return -1;

    int ris = 0;
    for (int i = 0; i < o->len; i++) {
        int block_index = i / SIZE;
        int bit_offset = i % SIZE;

        uint64_t bit_o = (o->data[block_index] >> bit_offset) & 1ULL;
        uint64_t bit_e = (e->data[block_index] >> bit_offset) & 1ULL;

        if (bit_o != bit_e)
            ris++;
    }

    return ris;
}

void shortcut(){
    struct mat *ar = read_key(A_PUB);
    struct mat *yr = read_key(Y_PUB);
    struct mat *sr = read_key(PRIVA);

    struct arr *msg = generate_random_message(L);
    write_packet(WRGEN, msg);

    encrypt_decrypt_shortcut(ar, yr, sr, msg, TEST1);
    encrypt_decrypt_shortcut(ar, yr, sr, msg, TEST2);
    encrypt_decrypt_shortcut(ar, yr, sr, msg, TEST3);

    free_resources(ar, yr, sr, NULL, msg, NULL, NULL);

    fprintf(stdout, "Trying to correct message.\n\n");

    struct arr *t1, *t2, *t3;

    t1 = read_packet(TEST1);
    t2 = read_packet(TEST2);
    t3 = read_packet(TEST3);

    if (t1 == NULL || t2 == NULL || t3 == NULL) {
        handle_error("Failed to read test packets.");
        return;
    }

    struct arr *corrected = correct_errors(t1, t2, t3);
    if (corrected == NULL) {
        handle_error("Failed to correct errors.");
        return;
    }

    fprintf(stdout, "After correcting code w/ three test...\n");
    print_hamming_distance(msg, corrected);
    write_packet(PLAIN, corrected);
}

void encrypt_decrypt_shortcut(struct mat *ar, struct mat *yr, struct mat *sr, struct arr *msg, const char *filename){
    struct arr *e = calloc(1ULL, sizeof(struct arr));
    if (e == NULL) {
        handle_error("Memory allocation failed for e struct.");
        return;
    }
    e->len = N;
    e->data = weight_array(N, T);
    if (e->data == NULL) {
        handle_error("Memory allocation failed for e data.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, NULL);
        return;
    }

    struct arr *w_nnc = mat_arr_mul(ar, e);
    if (w_nnc == NULL) {
        handle_error("Failed to multiply matrix with array.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, NULL);
        return;
    }

    struct arr *tmp_arr = mat_arr_mul(yr, e);
    if (tmp_arr == NULL) {
        handle_error("Failed to multiply matrix with array.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, w_nnc);
        return;
    }

    struct arr *w_word = array_xor(tmp_arr, msg);
    if (w_word == NULL) {
        handle_error("Failed to XOR arrays.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    struct arr *dec_key = mat_arr_mul(sr, w_nnc);
    if (dec_key == NULL) {
        handle_error("Failed to decrypt key.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    struct arr *dec_msg = array_xor(w_word, dec_key);
    if (dec_msg == NULL) {
        handle_error("Failed to decrypt message.");
        free_resources(NULL, NULL, NULL, NULL, msg, e, tmp_arr);
        return;
    }

    write_packet(filename, dec_msg);

    fprintf(stdout, "Finished %s.\n", filename);

    print_hamming_distance(msg, dec_msg);

    free_resources(NULL, NULL, NULL, NULL, e, dec_key, dec_msg);
}
