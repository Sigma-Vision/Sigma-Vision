#define CONFIG_FILE "xor.nnconf"
#define DEFAULT_CONFIG_FILE "default_xor.nnconf"

void write_config(int nb_inputs, int nb_layers, int* nb_nodes, int** biases,
                 int*** weights);
