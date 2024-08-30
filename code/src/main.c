#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "../include/test.h"
#include "../include/api.h"

//Command enumeration
typedef enum { GENERATE, ENCRYPT, DECRYPT, CORRECT, TEST, INVALID } Command;

Command get_command(const char *);
void print_err(const char *, const char *);
int set_paths(const char *);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [<args>]\n", argv[0]);
        return 1;
    }

    Command cmd = get_command(argv[1]);

    switch(cmd) {
        case TEST:
            test(argc > 2 ? atoi(argv[2]) : 0);
            break;

        case GENERATE:
            generate_key();
            break;

        case ENCRYPT:
            if (argc < 4) {
                print_err(argv[0], "encrypt <message> <key_a_path> <key_y_path>\n");
                return 2;
            }
            encrypt(argv[2], argv[3], argv[4]);
            break;

        case DECRYPT:
            if (argc < 4) {
                print_err(argv[0], "decrypt <nnc_path> <word_path> <key_path>\n");
                return 2;
            }
            decrypt(argv[2], argv[3], argv[4]);
            break;

        case CORRECT:
            if (argc < 4) {
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
    if (strcmp(command, "test") == 0)
        return TEST;
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
