#ifndef API_H
#define API_H

#include <stdint.h>

void generate_key(void);
void encrypt(const char *mex, const char *a_path, const char *y_path);
void decrypt(const char *fnnc, const char *fword, const char *key_path);
void correct(const char *path1, const char *path2, const char *path3);

#endif // API_H
