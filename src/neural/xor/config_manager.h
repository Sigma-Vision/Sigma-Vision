#define CONFIG_FILE "test.nnconf"
#define DEFAULT_CONFIG_FILE "default_xor.nnconf"

void write_config(const int nb_inputs, const int nb_layers,
                  const int nb_nodes[], double** biases_pp,
                  double*** weights_ppp);

int get_nb_layers();

int get_nb_inputs();

void get_nb_nodes(const int nb_layers, int nb_nodes[]);

void get_config(const int nb_inputs, const int nb_layers, const int nb_nodes[],
                double** biases_pp, double*** weights_ppp);
