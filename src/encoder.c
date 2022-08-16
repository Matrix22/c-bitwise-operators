#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM 8
#define MAX_DIM 16
#define FAIL_ENCODE 0
#define NUM_BITS 29
#define OP_BITS 2
#define DIM_BITS 4
#define INT_BITS 32

unsigned int encode_instr(void) {
    int check_scan = 0;
    unsigned int instr_num = 0;

    check_scan = scanf(" %u", &instr_num);

    if (check_scan > 1) {
        return FAIL_ENCODE;
    }

    if ((instr_num <= 0) || (instr_num > MAX_NUM)) {
        return FAIL_ENCODE;
    }

    unsigned int instr_dim = 0;

    check_scan = scanf(" %u", &instr_dim);

    if (check_scan > 1) {
        return FAIL_ENCODE;
    }

    if ((instr_dim <= 0) || (instr_dim > MAX_DIM)) {
        return FAIL_ENCODE;
    }

    unsigned int encode_ans = (instr_num - 1);

    unsigned char op = 0;
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

    encode_ans <<= 4;
    encode_ans |= (instr_dim - 1);

    encode_ans <<= (INT_BITS - 2 * instr_num - 7);

    return encode_ans;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of files\n");

        exit(EXIT_FAILURE);
    }

    FILE *fi = NULL;

    if ((fi = freopen(argv[1], "r", stdin)) == NULL) {
        fprintf(stderr, "Could not open file for reading encode\n");

        exit(EXIT_FAILURE);
    }

    unsigned int encode = encode_instr();

    fclose(fi);

    if ((fi = freopen(argv[2], "w", stdout)) == NULL) {
        fprintf(stderr, "Could not open file for writing encode\n");

        exit(EXIT_FAILURE);
    }

    printf("%u\n", encode);

    fclose(fi);

    return 0;
}