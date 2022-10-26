#define CONFIG_FILE "xor.nnconf"
#define DEFAULT_CONFIG_FILE "default_xor.nnconf"

void write_config(const int nb_inputs, const int nb_layers,
                  const int* nb_nodes_p, const int** biases_pp,
                  const int*** weights_ppp)

int get_nb_layers();

void get_nb_nodes(const int nb_layers, int* nb_nodes_p);

void get_config(const int nb_inputs, const int nb_layers, const int* nb_nodes_p,
                int** biases_pp, int*** weights_ppp);
