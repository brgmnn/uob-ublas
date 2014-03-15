#include <stdio.h>
#include "floatfann.h"

int main()
{
    fann_type *calc_out;
    fann_type input[2];

    struct fann *ann = fann_create_from_file("../atlas-plasma-float.net");

    // fann_print_connections(ann);
	// fann_print_parameters(ann);
	// return 0;

    // input[0] = 2*(5.0/100)-1;
    input[0] = 49;
    // input[1] = 2*(5.0/100)-1;
    input[1] = 49;

    // fann_reset_MSE(ann);
    // fann_scale_input(ann, input);
    calc_out = fann_run(ann, input);
    // fann_descale_output(ann, calc_out);

    printf("xor test (%f,%f) -> %f, %f\n", input[0], input[1], calc_out[0], calc_out[1]);

    fann_destroy(ann);
    return 0;
}