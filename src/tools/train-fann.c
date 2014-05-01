#include <stdio.h>
#include <fann/fann.h>
// #include <ublas.h>

int main() {
	printf("Neural Network training\n");

	const unsigned int num_input = 3;
    const unsigned int num_output = 4;
    const unsigned int num_layers = 4;
    const unsigned int num_neurons_hidden = 50;
    const float desired_error = (const float) 0.05;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1000;

    struct fann *ann = fann_create_standard(
		num_layers,
    	num_input,
    	num_neurons_hidden,
    	num_neurons_hidden,
    	num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    struct fann_train_data *data = fann_read_train_from_file("../training.data");

    fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error);
    fann_destroy_train(data);

    fann_save(ann, "../neural.net");

    fann_destroy(ann);

	return 0;
}
