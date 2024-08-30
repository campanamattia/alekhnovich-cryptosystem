#ifndef TEST_H
#define TEST_H

#include <stdint.h>
#include "arrays.h"

// Function prototypes
void test(void);

void handle_error(const char *message);
void free_resources(struct mat *m1, struct mat *m2, struct mat *m3, struct mat *m4, struct arr *a1, struct arr *a2, struct arr *a3);

struct arr* generate_random_message(int length);
void generate_matrices(struct mat **aw, struct mat **sw, struct mat **ew);
void check_key_reading(struct mat *aw, struct mat *yw, struct mat *sw);
void encryption_decryption_process(struct mat *ar, struct mat *yr, struct mat *sr, struct arr *msg);
void print_hamming_distance(struct arr *msg, struct arr *dec_msg);

int check_mat(struct mat *w, struct mat *r);
int check_arr(struct arr *w, struct arr *r);
int delta_arr(struct arr *o, struct arr *e);
#endif // TEST_H
