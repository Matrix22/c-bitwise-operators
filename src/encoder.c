#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Defines for number of bits
 * needed by the program to work (magic numbers)
 * 
 */
#define MAX_NUM 8
#define MAX_DIM 16
#define FAIL_ENCODE 0
#define NUM_BITS 29
#define OP_BITS 2
#define DIM_BITS 4
#define INT_BITS 32

/**
 * @brief Function that will encode your instruction
 * so the program can understand it. 
 * 
 * @return unsigned int the instruction incoded
 */
unsigned int encode_instr(void) {
    int check_scan = 0;
    unsigned int instr_num = 0;

    /* Read the instruction number */
    check_scan = scanf(" %u", &instr_num);

    if (check_scan > 1) {
        return FAIL_ENCODE;
    }

    /* Check if instruction number is valid */
    if ((instr_num <= 0) || (instr_num > MAX_NUM)) {
        return FAIL_ENCODE;
    }

    unsigned int instr_dim = 0;

    /* Read the instruction dimension */
    check_scan = scanf(" %u", &instr_dim);

    if (check_scan > 1) {
        return FAIL_ENCODE;
    }

    /* Check if instruction dimension is valid */
    if ((instr_dim <= 0) || (instr_dim > MAX_DIM)) {
        return FAIL_ENCODE;
    }

    /* Initialize the encode instruction (encode number) */
    unsigned int encode_ans = (instr_num - 1);
    unsigned char op = 0;

    /* Encode the operations */
    for (unsigned int iter = 0; iter < instr_num; ++iter) {
        check_scan = scanf(" %c", &op);

        if (check_scan > 1) {
            return FAIL_ENCODE;
        }
        
        encode_ans <<= OP_BITS;
        
        if (op == '+') {
            encode_ans |= 0;
        } else if (op == '-') {
            encode_ans |= 1;
        } else if (op == '*') {
            encode_ans |= 2;
        } else if (op == '/') {
            encode_ans |= 3;
        } else {
            
            return FAIL_ENCODE;
        }
    }

    /* Encode instruction dimension */
    encode_ans <<= 4;
    encode_ans |= (instr_dim - 1);

    /* Mathe the instruction to be valid */
    encode_ans <<= (INT_BITS - 2 * instr_num - 7);

    /* Return the encode instruction */
    return encode_ans;
}

int main(int argc, char *argv[]) {
    /* Function has to have two files to read and to write */
    if (argc != 3) {
        fprintf(stderr, "Wrong number of files\n");

        exit(EXIT_FAILURE);
    }

    /* Open file for reading the input */
    FILE *fi = NULL;

    if ((fi = freopen(argv[1], "r", stdin)) == NULL) {
        fprintf(stderr, "Could not open file for reading encode\n");

        exit(EXIT_FAILURE);
    }

    /* Find the encoded instruction */
    unsigned int encode = encode_instr();

    fclose(fi);

    /* Open file for writing */
    if ((fi = freopen(argv[2], "w", stdout)) == NULL) {
        fprintf(stderr, "Could not open file for writing encode\n");

        exit(EXIT_FAILURE);
    }

    /* Print the encoded instruction */
    printf("%u\n", encode);

    fclose(fi);

    return 0;
}