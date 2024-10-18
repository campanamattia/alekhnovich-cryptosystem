# Alekhnovich's Cryptosystem

## Project Overview
This project implements Alekhnovich's cryptosystem, a quantum-resistant encryption method based on the difficulty of decoding random linear codes. As quantum computing continues to evolve, traditional cryptographic systems are increasingly at risk, and Alekhnovich's cryptosystem provides a robust alternative for securing sensitive data. The aim of this project is to explore its theoretical underpinnings, practical implementation, and potential as a post-quantum cryptographic solution.

The Alekhnovich cryptosystem is designed to ensure the security of encrypted messages even against potential quantum computers, which can efficiently solve problems that are currently infeasible for classical computers. The main concept involves leveraging the complexity of linear code decoding, which is an NP-hard problem, making it highly resistant to both classical and quantum attacks.

## Project Structure
The project is composed of multiple components that facilitate the cryptographic processes and provide flexibility for future developments. Below is an overview of the file structure and functionalities.

### File Structure
- **CMakeLists.txt**: Defines the build configuration, manages dependencies, includes directories, and links required libraries.
- **Header Files**: Located in the `include` directory, these files contain function declarations, macros, and data structures used across the project:
  - `alekhnovich.h`: Defines the main cryptographic functions and structures used for key generation, encryption, and decryption.
  - `api.h`: Provides the interface for interacting with the cryptosystem, allowing other components or users to easily utilize its functions.
  - `arrays.h`, `backend.h`, `bitop.h`, `seed.h`, `test.h`, `xoshiro.h`: Provide utility functions and low-level operations essential for handling array manipulations, bitwise operations, random seed management, and random number generation.
- **Source Files**: The implementation files (`.c`) are located in the `src` directory and contain the definitions for the cryptographic operations, including key generation, encryption, decryption, and helper functions.
- **Executable**: The compiled executable is generated in the `bin` directory after a successful build, which includes all the features of the cryptosystem.

### CMake Configuration
The `CMakeLists.txt` file plays a crucial role in configuring the build process:
- Specifies the minimum required version of CMake and sets the C standard to C99.
- Includes header files located in the `include` directory for the build process.
- Uses `file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")` to gather all `.c` files in the `src` directory as source files for the project.
- If no source files are found, the build process raises a fatal error.
- Creates an executable named `MyProject` from the gathered source files and links it with the `randombytes` library for secure random number generation.
- Configures the `LD_LIBRARY_PATH` environment variable to include the necessary library paths for runtime linking.
- Specifies the output directory for the compiled executable, ensuring it is placed in the `bin` folder.

## Functionality Overview
The project implements core functionalities of the Alekhnovich cryptosystem, focusing on key generation, encryption, decryption, and error correction. The following sections provide an overview of these components and how they work together to achieve secure communication.

### Key Components
- **Key Generation**: This step involves generating the key pair, which includes a private key matrix `S` and a public key matrix `Y`. Matrix `A` serves as a generator matrix for a random linear code, while matrix `E` is used to introduce noise, ensuring the security of the cryptosystem. The key generation is essential for maintaining the robustness of the system against quantum attacks.
- **Encryption**: Encryption is performed by generating a random vector `e` and combining it with the message and the public key matrix `Y`. The random vector adds complexity to the encrypted message, making it computationally difficult for adversaries to decode the ciphertext without the private key.
- **Decryption**: Decryption leverages the private key matrix `S` to decrypt the ciphertext and recover the original message. The error correction mechanism helps mitigate the impact of noise introduced during encryption, ensuring the integrity of the decrypted message.
- **Helper Functions**: Several helper functions are implemented to handle matrix manipulation, bitwise operations, random seed initialization, and random number generation. These include `bitop.h` for bitwise operations, `arrays.h` for handling array-related tasks, and `xoshiro.h` for efficient pseudo-random number generation.

## Most Enlighting Algorithms

### Transposition Algorithm
```c
struct mat *matrix_transpose(struct mat *m) {
    if (m == NULL)
        return NULL;

    struct mat *t = calloc(1, sizeof(struct mat));
    if (t == NULL)
        return NULL;

    t->rows = m->cols;
    t->cols = m->rows;

    t->data = calloc(t->rows, sizeof(uint64_t *));
    if (t->data == NULL) {
        free(t);
        return NULL;
    }

    for (int i = 0; i < t->rows; i++) {
        t->data[i] = calloc(real_dim(t->cols), sizeof(uint64_t));
        if (t->data[i] == NULL) {
            free_mat(t);
            return NULL;
        }
    }

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            int bit_pos_in_block = j % SIZE;
            int block_index = j / SIZE;

            if (m->data[i][block_index] & (1ULL << bit_pos_in_block)) {
                int transpose_block_index = i / SIZE;
                int transpose_bit_pos_in_block = i % SIZE;

                t->data[j][transpose_block_index] |= (1ULL << transpose_bit_pos_in_block);
            }
        }
    }

    return t;
}
```
### Matrix Multiply Algorithm
```c
struct mat *matrix_mul(struct mat *a, struct mat *b) {
    if (a == NULL || b == NULL || a->cols != b->rows)
        return NULL;

    struct mat *c = calloc(1, sizeof(struct mat));
    if (c == NULL)
        return NULL;

    c->rows = a->rows;
    c->cols = b->cols;

    c->data = calloc(c->rows, sizeof(uint64_t *));
    if (c->data == NULL) {
        free(c);
        return NULL;
    }

    for (int i = 0; i < c->rows; i++) {
        c->data[i] = calloc(real_dim(c->cols), sizeof(uint64_t));
        if (c->data[i] == NULL) {
            free_mat(c);
            return NULL;
        }
    }

    struct mat *bt = matrix_transpose(b);
    if (bt == NULL) {
        free_mat(c);
        return NULL;
    }

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < bt->rows; j++) {
            c->data[i][j / SIZE] |= shift_bit(bax(a->data[i], bt->data[j], real_dim(a->cols)), j % SIZE);
        }
    }

    free_mat(bt);
    return c;
}
```
### Encrypt Algorithm
```c
void encrypt(uint64_t *mex, const char *a_path, const char *y_path) {
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
    message->data = mex;

    word = array_xor(message, tmp);

    if(word->data == NULL)
        return;

    write_packet(WRNNC, nnc);
    write_packet(ENCRY, word);
}
```

## Building and Running the Project
To build the project, follow these steps:
```sh
mkdir build
cd build
cmake ..
make
```
This will create the executable in the `bin` directory. Run the executable as follows:
```sh
./bin/MyProject
```
Ensure that the `randombytes` library is installed and available in your system's library path to avoid runtime issues.

## Future Developments
- **Quantum Computing**: With advancements in quantum computing, it is crucial to continue researching and developing quantum-resistant cryptographic algorithms. Alekhnovich's cryptosystem has the potential to become a standard in post-quantum cryptography, but further work is required to validate its security and efficiency.
- **Standardization**: Future work should also focus on integrating this cryptosystem with existing cryptographic standards. This will facilitate a smoother transition for organizations seeking to adopt quantum-resistant solutions.
- **Performance Optimization**: Optimizing the performance of key generation, encryption, and decryption processes is a key area for future improvement. Reducing computational overhead while maintaining security is essential to make this cryptosystem practical for real-world applications.
- **Testing and Validation**: Expanding test coverage to include various scenarios and edge cases will help validate the robustness of the implementation. Future iterations can include fuzz testing, stress testing, and integration with other cryptographic systems.

## Conclusion
The Alekhnovich cryptosystem presents a promising solution for securing data against quantum computing threats. This project provides a detailed exploration of its theoretical basis, practical implementation, and potential real-world applications. The key generation, encryption, and decryption processes have been implemented and tested to demonstrate the cryptosystem's capabilities. Further research and development are needed to improve the performance and integration of this cryptosystem with existing security standards.

## Screenshots and Diagrams
![example](https://github.com/campanamattia/alekhnovich-cryptosystem/blob/master/report/ppt/img/code_screen/shell_1.png)

## References
1. Alekhnovich, M. (2011). *More on average case vs approximation complexity*. Comput. Complex., 20(4):755–786.
2. Bernstein, D. J. (2023). *librandombytes: Cryptographically secure random number generator*. [Online](https://randombytes.cr.yp.to/index.html)
3. Blackman, D., & Vigna, S. (2021). *Scrambled linear pseudorandom number generators*. ACM Trans. Math. Softw., 47(4):36:1–36:32.

