#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct nand {
    bool *in_a, *in_b;
    bool out;
} nand_t;

int main(void) {
    bool in_a = false;
    bool in_b = false;
    nand_t nand = {
        .in_a = &in_a,
        .in_b = &in_b,
    };
    printf("Simulating nand with %d and %d\n", *nand.in_a, *nand.in_b);
    nand.out = !(*nand.in_a & *nand.in_b);
    printf("Result: %d\n", nand.out);
    return 0;
}