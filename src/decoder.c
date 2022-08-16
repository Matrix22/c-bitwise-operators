#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SHORT_BITS 16
#define NUM_BITS 29
#define DIM_MASK 15
#define OP_BITS 2
#define FAIL_EXEC INT64_MAX

struct proc_instr_s {
    unsigned char instr_num : 3;
    unsigned char instr_dim : 4;
    unsigned char instr_op[8];
};

void instr_decorder(unsigned int instr, struct proc_instr_s *proc_instr) {
    if ((instr == 0) || (proc_instr == NULL)) {
        return;
    }

    proc_instr->instr_num = (instr >> NUM_BITS);
    proc_instr->instr_dim = (instr << (5 + 2 * proc_instr->instr_num)) >> (NUM_BITS - 1);

    instr >>= (NUM_BITS - 2 - 2 * proc_instr->instr_num);

    unsigned int op_mask = (3 << (2 * proc_instr->instr_num));

    for (unsigned char iter = 0; iter <= proc_instr->instr_num; ++iter) {
        proc_instr->instr_op[iter] = (instr & op_mask) >> (2 * proc_instr->instr_num);

        instr <<= OP_BITS;
    }
}

void instr_show(struct proc_instr_s *proc_instr) {
    if (proc_instr == NULL) {
        return;
    }

    printf("%d ", 1 + proc_instr->instr_num);

    for (unsigned char iter = 0; iter <= proc_instr->instr_num; ++iter) {
        if (proc_instr->instr_op[iter] == 0) {
            printf("+ ");
        } else if (proc_instr->instr_op[iter] == 1) {
            printf("- ");
        } else if (proc_instr->instr_op[iter] == 2) {
            printf("* ");
        } else {
            printf("/ ");
        }
    }

    printf("%d\n", 1 + proc_instr->instr_dim);
}

long long int instr_pow_2_dim_no_precedence_exec(struct proc_instr_s *proc_instr) {
    if (proc_instr == NULL) {
        return FAIL_EXEC;
    }

    unsigned int read_op = (2 + proc_instr->instr_num) * (1 + proc_instr->instr_dim);

    if (read_op % SHORT_BITS != 0) {
        read_op = (read_op / SHORT_BITS) + 1; 
    } else {
        read_op /= SHORT_BITS;
    }

    long long int exec_ans = INT64_MAX;
    int iter_k = 0;
    unsigned short opr = 0;
    unsigned int opr_num = SHORT_BITS / (1 + proc_instr->instr_dim);

    for (unsigned int iter_i = 1; iter_i <= read_op; ++iter_i) {
        int check_scan = scanf("%hu", &opr);

        if (check_scan > 1) {
            return FAIL_EXEC;
        }

        for (unsigned int iter_j = 0; iter_j < opr_num; ++iter_j) {
            unsigned short exec_val = (opr >> (SHORT_BITS - 1 - proc_instr->instr_dim));

            if ((iter_j == 0) && (exec_ans == INT64_MAX)) {
                exec_ans = exec_val;
            } else {
                if (iter_k <= proc_instr->instr_num) {
                    if (proc_instr->instr_op[iter_k] == 0) {
                        exec_ans += exec_val;
                    } else if (proc_instr->instr_op[iter_k] == 1) {
                        exec_ans -= exec_val;
                    } else if (proc_instr->instr_op[iter_k] == 2) {
                        exec_ans *= exec_val;
                    } else {
                        exec_ans /= exec_val;
                    }

                    ++iter_k;
                }
            }

            opr <<= (1 + proc_instr->instr_dim);
        }
    }

    return exec_ans;
}

long long int instr_no_precedence_exec(struct proc_instr_s *proc_instr) {
    if (proc_instr == NULL) {
        return FAIL_EXEC;
    }

    unsigned int read_op = (2 + proc_instr->instr_num) * (1 + proc_instr->instr_dim);

    if (read_op % SHORT_BITS != 0) {
        read_op = (read_op / SHORT_BITS) + 1; 
    } else {
        read_op /= SHORT_BITS;
    }

    long long int exec_ans = INT64_MAX;
    int iter_k = 0;
    unsigned short opr = 0;
    unsigned int opr_num = SHORT_BITS / (1 + proc_instr->instr_dim);
    unsigned char remain_bits = 0;
    unsigned short opr_rest = 0;
    unsigned int iter_i = read_op;
    int check_scan = 0;

    while (iter_i >= 1) {
        if (remain_bits == 0) {
            check_scan = scanf("%hu", &opr);

            if (check_scan > 1) {
                return FAIL_EXEC;
            }

            opr_rest = opr;

            --iter_i;
        } else if (remain_bits < 1 + proc_instr->instr_dim){
            opr_rest = opr << (SHORT_BITS - remain_bits);
            
            check_scan = scanf("%hu", &opr);

            if (check_scan > 1) {
                return FAIL_EXEC;
            }
            
            opr_rest = opr_rest | (opr >> remain_bits);

            --iter_i;
        } else {
            opr_rest = opr << (SHORT_BITS - remain_bits);
        }

        for (unsigned int iter_j = 0; iter_j < opr_num; ++iter_j) {
            unsigned short exec_val = (opr_rest >> (SHORT_BITS - 1 - proc_instr->instr_dim));

            if ((iter_j == 0) && (exec_ans == INT64_MAX)) {
                exec_ans = exec_val;
            } else {
                if (iter_k <= proc_instr->instr_num) {
                    if (proc_instr->instr_op[iter_k] == 0) {
                        exec_ans += exec_val;
                    } else if (proc_instr->instr_op[iter_k] == 1) {
                        exec_ans -= exec_val;
                    } else if (proc_instr->instr_op[iter_k] == 2) {
                        exec_ans *= exec_val;
                    } else {
                        exec_ans /= exec_val;
                    }

                    ++iter_k;
                }
            }

            opr_rest <<= (1 + proc_instr->instr_dim);
        }

        remain_bits = (remain_bits + SHORT_BITS - 1 - proc_instr->instr_dim) % SHORT_BITS;
    }

    return exec_ans;
}

long long int instr_exec(struct proc_instr_s *proc_instr) {
    if (proc_instr == NULL) {
        return FAIL_EXEC;
    }

    unsigned int read_op = (2 + proc_instr->instr_num) * (1 + proc_instr->instr_dim);

    unsigned long long int exec_array[2 + proc_instr->instr_num]; 

    if (read_op % SHORT_BITS != 0) {
        read_op = (read_op / SHORT_BITS) + 1; 
    } else {
        read_op /= SHORT_BITS;
    }

    int iter_exec = 0;
    unsigned short opr = 0;
    unsigned int opr_num = SHORT_BITS / (1 + proc_instr->instr_dim);
    unsigned char remain_bits = 0;
    unsigned short opr_rest = 0;
    unsigned int iter_i = read_op;
    int check_scan = 0;

    while (iter_i >= 1) {
        if (remain_bits == 0) {
            check_scan = scanf("%hu", &opr);

            if (check_scan > 1) {
                return FAIL_EXEC;
            }

            opr_rest = opr;

            --iter_i;
        } else if (remain_bits < 1 + proc_instr->instr_dim){
            opr_rest = opr << (SHORT_BITS - remain_bits);
            
            check_scan = scanf("%hu", &opr);

            if (check_scan > 1) {
                return FAIL_EXEC;
            }
            
            opr_rest = opr_rest | (opr >> remain_bits);

            --iter_i;
        } else {
            opr_rest = opr << (SHORT_BITS - remain_bits);
        }

        for (unsigned int iter_j = 0; iter_j < opr_num; ++iter_j) {
            exec_array[iter_exec] = (opr_rest >> (SHORT_BITS - 1 - proc_instr->instr_dim));

            ++iter_exec;
            opr_rest <<= (1 + proc_instr->instr_dim);
        }

        remain_bits = (remain_bits + SHORT_BITS - 1 - proc_instr->instr_dim) % SHORT_BITS;
    }

    unsigned int exec_array_size = 2 + proc_instr->instr_num;
    unsigned int op_array_size = exec_array_size - 1;

    for (unsigned int iter = 0; iter < op_array_size;) {
        if ((proc_instr->instr_op[iter] == 2) || (proc_instr->instr_op[iter] == 3)) {
            if (proc_instr->instr_op[iter] == 2) {
                exec_array[iter] *= exec_array[iter + 1];
            } else {
                exec_array[iter] /= exec_array[iter + 1];
            }

            --exec_array_size;
            for (unsigned int perm = iter + 1; perm < exec_array_size; ++perm) {
                exec_array[perm] = exec_array[perm + 1];
            }

            --op_array_size;
            for (unsigned int perm = iter; perm < op_array_size; ++perm) {
                proc_instr->instr_op[perm] = proc_instr->instr_op[perm + 1];
            }
        } else {
            ++iter;
        }
    }

    long long int exec_ans = exec_array[0];

    for (unsigned iter = 0; iter < op_array_size; ++iter) {
        if (proc_instr->instr_op[iter] == 0) {
            exec_ans += exec_array[iter + 1];
        } else {
            exec_ans -= exec_array[iter + 1];
        }
    }

    return exec_ans;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Wrong number of files\n");

        exit(EXIT_FAILURE);
    }

    FILE *fin = NULL;

    if ((fin = freopen(argv[1], "r", stdin)) == NULL) {
        fprintf(stderr, "Could not open file for reading decode\n");

        exit(EXIT_FAILURE);
    }

    FILE *fout = NULL;

    if ((fout = freopen(argv[2], "w", stdout)) == NULL) {
        fprintf(stderr, "Could not open file for writing decode\n");

        exit(EXIT_FAILURE);
    }

    unsigned int instr = 0;

    if (scanf("%u", &instr) > 1) {
        fclose(fin);
        fclose(fout);

        fprintf(stderr, "Reading instruction failed\n");

        exit(EXIT_FAILURE);
    }

    struct proc_instr_s PR;
    instr_decorder(instr, &PR);

    long long int exec_ans = INT64_MAX;

    int exec_mod = atoi(argv[3]);

    if (exec_mod == 0) {
        instr_show(&PR);
    } else if (exec_mod == 1) {
        exec_ans = instr_pow_2_dim_no_precedence_exec(&PR);
    } else if (exec_mod == 2) {
        exec_ans = instr_no_precedence_exec(&PR);
    } else if (exec_mod == 3) {
        exec_ans = instr_exec(&PR);
    }

    if (exec_ans == INT64_MAX) {
        printf("No answer calculated\n");
    } else {
        printf("%lld\n", exec_ans);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}