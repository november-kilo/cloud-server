#include <Array.h>

inherit "/lib/util/statistics/sampling";

int sample_size;
mapping sample_map;

static void create(int sample_size) {
    ::sample_size = sample_size;
    ::sample_map = ([
        "normal": "box_muller_normal",
        "normal-mix": "sample_mixture_of_normals",
        "uniform": "sample_uniform",
        "exp": "sample_exponential",
        "gamma": "sample_gamma",
        "inverse-gamma": "sample_inverse_gamma"
    ]);
}

private string find_sampler(string sampler_key) {
    int i;
    string sampler;
    Array indices;

    indices = new Array(map_indices(sample_map));
    ({ i, sampler }) = indices->find_first(sampler_key);

    if (i == -1) {
        error("Invalid sampler key: " + sampler_key);
    }

    return sampler;
}

private string get_sampler(string sampler_key) {
    string sampler;

    sampler = find_sampler(sampler_key);

    return sample_map[sampler];
}

private float *create_data(int sample_size, string sampler, float args...) {
    float *data;
    int i;

    data = allocate_float(sample_size);
        
    for (i = 0; i < sample_size; i++) {
        data[i] = call_other(this_object(), sampler, args...);
    }

    return data; 
}

float *generate_distribution(string value_generator, float args...) {
    string sampler;

    sampler = get_sampler(value_generator);
    return sampler == nil
        ? nil
        : create_data(sample_size, sampler, args...);
}
