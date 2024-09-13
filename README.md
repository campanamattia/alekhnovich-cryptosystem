# Alekhnovich Cryptosystem

This repository contains the implementation of Alekhnovich's Cryptosystem, a post-quantum cryptographic system based on the hardness of decoding random linear codes, which is an NP-hard problem believed to be resistant to quantum attacks. This project provides the necessary tools to generate keys, encrypt and decrypt messages, and correct errors, using a C-based implementation.

## Overview

Alekhnovich’s Cryptosystem offers a quantum-resistant approach to encryption by leveraging the complexity of decoding random linear codes. This cryptosystem could be a critical component of future-proof data security solutions, especially in the context of advancing quantum computing technologies.

### Key Components

- **Key Generation:** Creates the public and private keys needed for encryption and decryption.
- **Encryption:** Encrypts a message using the generated public key.
- **Decryption:** Decrypts the message using the private key.
- **Error Correction:** Provides mechanisms to correct errors that may occur during data transmission.

## Installation

### Prerequisites

- **CMake:** To build the project.
- **librandombytes:** A cryptographically secure random number generator, which must be installed before building the project.

### Steps to Install

1. **Download and install `librandombytes`:**

   ```bash
   wget -m https://randombytes.cr.yp.to/librandombytes-latest-version.txt
    version=$(cat randombytes.cr.yp.to/librandombytes-latest-version.txt)
    wget -m https://randombytes.cr.yp.to/librandombytes-$version.tar.gz
    tar -xzf randombytes.cr.yp.to/librandombytes-$version.tar.gz
    cd librandombytes-$version
   ```
   To install in _/usr/local/{include,lib,bin}_:
   ```bash
   ./configure && make -j8 install
   ```

3. **Build the Project:**

   Navigate to the root directory of the project and create a build directory:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Run the Program:**

   After building the project, you can run the program using:

   ```bash
   ./your_program_name <command> [<args>]
   ```

## Usage

The program supports several commands that you can execute from the command line:

- **TEST:** Run the test suite.
- **GENERATE:** Generate encryption keys.
- **ENCRYPT:** Encrypt a message.
- **DECRYPT:** Decrypt a message.
- **CORRECT:** Perform error correction.

### Example Commands

- **Generate Keys:**

  ```bash
  ./your_program_name generate
  ```

- **Encrypt a Message:**

  ```bash
  ./your_program_name encrypt <message> <public_key_path> <output_path>
  ```

- **Decrypt a Message:**

  ```bash
  ./your_program_name decrypt <encrypted_file_path> <private_key_path> <output_path>
  ```

- **Run Tests:**

  ```bash
  ./your_program_name test
  ```

## Project Structure

- **main.c:** The entry point of the program, handling command-line arguments and invoking corresponding functions.
- **test.c:** Contains test functions to validate cryptographic processes.
- **api.c:** Provides core API functions for key generation, encryption, decryption, and error correction.
- **backend.c:** Handles low-level data operations, including reading/writing keys and performing error correction.
