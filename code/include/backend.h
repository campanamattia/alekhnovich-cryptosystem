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
void write_key(const char *path, struct mat *m);
uint64_t *convert_to_array(const char *filepath);
struct mat *read_key(const char *path);
void write_packet(const char *output_path, struct arr *message);
struct arr *read_packet(const char *input_path);
struct arr *correct_errors(struct arr *, struct arr *, struct arr *);

#endif // ALEKHNOVICH_H
