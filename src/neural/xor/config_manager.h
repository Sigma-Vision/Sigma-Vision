#define CONFIG_FILE "xor.nnconf"
#define DEFAULT_CONFIG_FILE "default_xor.nnconf"

void write_config(int nb_inputs, int nb_layers, int* nb_nodes, int** biases,
                 int*** weights);

int get_nb_layers();

void get_nb_nodes(int nb_layers, int* nb_nodes_p);