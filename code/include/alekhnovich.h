#ifndef ALEKHNOVICH_H
#define ALEKHNOVICH_H

#include <stdint.h>
#include <stdio.h>
#include "arrays.h"

// Constants
#define L 13000
#define T 144
#define K 1300
#define N 16000

// Paths
#define A_PUB "target/a_pub.bin"
#define Y_PUB "target/y_pub.bin"
#define PRIVA "target/priva.bin"

#define WRGEN "target/wrgen.bin"
#define WRNNC "target/wrnnc.bin"
#define ENCRY "target/encry.txt"

#define NOISY "target/noisy.txt"
#define PLAIN "target/plain.txt"
#define BINEX "target/binex.txt"


// Function prototypes
void generate_key(void);
void write_key(const char *path, struct mat *m);
uint64_t *convert_to_array(const char *filepath);
void encrypt(uint64_t *mex, const char *a_path, const char *y_path);
struct mat *read_key(const char *path);
void write_packet(const char *output_path, struct arr *message);
void decrypt(const char *fnnc, const char *fword, const char *key_path);
struct arr *read_packet(const char *input_path);
void correct(const char *path1, const char *path2, const char *path3);
struct arr *read_word(FILE *fp);

#endif // ALEKHNOVICH_H
