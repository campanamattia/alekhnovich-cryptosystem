#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "seed.h"
#include "bitop.h"
#include "xoshiro.h"

static size_t size_64 = sizeof(uint64_t);

/*Returns a matrix of random uint64_t values
*/
uint64_t **random_matrix(int rows, int cols){
    uint64_t **matrix = (uint64_t **) calloc(rows, sizeof(uint64_t *));

    if(matrix == NULL){
        printf("Error allocating memory\n");
        return NULL;
    }

    init_seed();

    for(int i = 0; i < rows; i++){
        matrix[i] = (uint64_t *) calloc(cols, sizeof(uint64_t));

        if(matrix[i] == NULL){
            printf("Error allocating memory\n");
            return NULL;
        }

        for(int j = 0; j < cols; j++){
            matrix[i][j] = next();
        }
    }

    return matrix;
}

/*Returns a matrix with rows with fixed 1s in random positions
*/
uint64_t **weighted_matrix(int rows, int cols, int weight){
    uint64_t **matrix = (uint64_t **) calloc(rows, sizeof(uint64_t *));

    if(matrix == NULL){
        printf("Error allocating memory\n");
        return NULL;
    }

    for(int i = 0; i < rows; i++){
        matrix[i] = weighted_array(cols, weight);
    }

    return matrix;
}

/*Returns an array with fixed 1s in random positions
*/
uint64_t *weighted_array(int size, int weight){
    uint64_t *array = (uint64_t *) calloc(size, size_64);

    if(array == NULL){
        printf("Error allocating memory\n");
        return NULL;
    }

    init_seed();

    for(int k = 0; k < weight; k++){
        int i, s;

        do{
            uint64_t p = next();
            i = p / sizeof(uint64_t);
            s = p % sizeof(uint64_t);
        } while(fetch_bit(array[i], s));

        array[i] |= 1 << s;
    }

    return array;
}

/*Returns the transposed matrix bit-wise
*/
uint64_t **transpose(uint64_t **matrix, int rows, int cols){
    uint64_t **transposed = (uint64_t **) calloc(cols, sizeof(uint64_t *));

    if(transposed == NULL){
        return NULL;
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            for(int k = 0; k < size_64; k++){
                transposed[j * 64 + k][i / size_64] |= fetch_bit(matrix[i][j], k) << (i % size_64);
            }
        }
    }

    return transposed;
}

uint64_t **matrix_product(uint64_t **A, uint64_t **B, int a_rows, int a_cols, int b_rows, int b_cols){
    if(a_cols != b_rows)
        return NULL;

    uint64_t **bt = transpose(B, b_rows, b_cols);

    if(bt == NULL)
        return NULL;

    uint64_t **product = (uint64_t **) calloc(a_rows, sizeof(uint64_t *));

    if(product == NULL)
        return NULL;

    for(int i = 0; i < a_rows; i++){
        for(int j = 0;  j < b_cols; j++){
            int bit = bax(A[i], bt[j], a_cols);
            product[i][j / size_64] |= bit << (j % size_64);
        }
    }

    return product;
}

/*Returns the sum of two matrices bit-wise
*/
uint64_t **matrix_sum(uint64_t **A, uint64_t **B, int rows, int cols){
    uint64_t **sum = (uint64_t **) calloc(rows, sizeof(uint64_t *));

    if(sum == NULL){
        return NULL;
    }

    for(int i = 0; i < rows; i++){
        sum[i] = (uint64_t *) calloc(cols, sizeof(uint64_t));

        if(sum[i] == NULL){
            return NULL;
        }

        for(int j = 0; j < cols; j++){
            sum[i][j] = A[i][j] ^ B[i][j];
        }
    }

    return sum;
}