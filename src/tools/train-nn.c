#include <stdio.h>

#include <ublas.h>

int main() {
	printf("Neural Network training\n");

	ublas_nn nn;

	int i;
	int neurons[3] = {2,3,1};

	ublas_nn_init(&nn, neurons, 3);

	ublas_nn_print(&nn, stdout);

	// for (int i=0; i < nn.; i++) {
	// 	nn.weights[i] = (double)i;
	// 	printf("weight = %f\n", nn.weights[i]);
	// }

	return 0;
}
