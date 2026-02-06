#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct nand {
    struct nand *in_a, *in_b;
    bool out;
} nand_t;

// basic recursive simulation
void simulate(nand_t *nand) {
    // requires simulating previous nand
    if (nand->in_a >= (struct nand*)2) {
        printf("Simulating above chip for input A\n");
        simulate(nand->in_a);
        nand->in_a = (struct nand *)nand->in_a->out;
    }
    if (nand->in_b >= (struct nand*)2) {
        printf("Simulating above chip for input B\n");
        simulate(nand->in_b);
        nand->in_b = (struct nand *)nand->in_b->out;
    }

    printf("NAND inputs: %d and %d = %d\n", (bool)nand->in_a, (bool)nand->in_b, !((bool)nand->in_a & (bool)nand->in_b));

    // now the inputs are constant (boolean not pointers)
    nand->out = !((bool)nand->in_a & (bool)nand->in_b);
}

int main(void) {
    nand_t a = {
        .in_a = (struct nand *)false,
        .in_b = (struct nand *)true,
    };
    nand_t b = {
        .in_a = &a,
        .in_b = (struct nand *)true,
    };

    printf("Simulating nand chain\n");
    
    simulate(&b);
    
    printf("Result: %d\n", b.out);
    return 0;
}