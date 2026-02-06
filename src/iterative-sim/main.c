#include <stdio.h>
#include <stdbool.h>

typedef struct nand {
    bool input_a, input_b;
    struct nand **out_chip;
    bool **out_chip_in;
    int num_outputs;
} nand;

void simulate(nand *nand) {
    bool res = !(nand->input_a & nand->input_b);
    for (int i = 0; i < nand->num_outputs; i++) {
        if (nand->out_chip_in[i] && res != *nand->out_chip_in[i]) {
            *nand->out_chip_in[i] = res;
            if (nand->out_chip[i]) simulate(nand->out_chip[i]);
        }
    }
    
    // printf("NAND SIMULATED: %d and %d is %d\n", nand->input_a, nand->input_b, res);
}

int main(void) {
    nand b = {
        .input_a = true,
        // above chip computes B
        .out_chip = NULL,
        .out_chip_in = NULL,
        .num_outputs = 0,
    };

    nand a = {
        .input_a = true,
        .input_b = true,
        .out_chip = (nand *[]){ &b },
        .out_chip_in = (bool *[]){ &b.input_b },
        .num_outputs = 1,
    };

    // stress test!!!
    for (int i = 0; i < 1000000; i++) {
        simulate(&a);
    }
}