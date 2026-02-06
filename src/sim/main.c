#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct nand {
    bool i_a, i_b, o_a;
} nand_t;

int main(void) {
    nand_t nand = {
        .i_a = false,
        .i_b = false,
    };
    printf("Simulating nand with inputs %d and %d...\n", nand.i_a, nand.i_b);
    nand.o_a = !(nand.i_a & nand.i_b);
    printf("Result: %d\n", nand.o_a);
    return 0;
}