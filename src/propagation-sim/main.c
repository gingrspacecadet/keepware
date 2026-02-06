#include <stdio.h>
#include <stdbool.h>

typedef struct nand {
    bool input_a, input_b;
    bool prev_out;
    struct nand *out_chip;
    bool *out_chip_in;
} nand;

void simulate(nand *nand) {
    bool res = !(nand->input_a & nand->input_b);
    nand->prev_out = res;
    if (nand->out_chip_in && res != *nand->out_chip_in) *nand->out_chip_in = res;
    printf("NAND SIMULATED: %d and %d is %d\n", nand->input_a, nand->input_b, res);

    if (nand->out_chip) simulate(nand->out_chip);
}

int main(void) {
    nand b = {
        .input_a = true,
        // above chip computes B
        .out_chip = NULL,
        .out_chip_in = NULL,
    };

    nand a = {
        .input_a = true,
        .input_b = true,
        .out_chip = &b,
        .out_chip_in = &b.input_b
    };

    simulate(&a);
}