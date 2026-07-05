#include <limits.h>

private float spare_normal;
private int has_spare_normal;

#define PI acos(-1.0)

/*
 * Uniform random float in [0, 1).
 */
float random_float() {
    return (float)random(0) / (float)INT_MAX;
}

/*
 * Standard normal sample via the Box-Muller transform, scaled to the
 * given mean/std_dev. Each call to the transform produces two
 * independent samples; the second is cached in spare_normal and
 * returned on the following call instead of recomputing.
 */
float box_muller_normal(float mean, float std_dev) {
    if (has_spare_normal) {
        has_spare_normal = 0;
        return mean + std_dev * spare_normal;
    } else {
        float u1, u2, z0, z1;
        u1 = random_float();
        u2 = random_float();
        
        z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
        z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * PI * u2);
        
        spare_normal = z1;
        has_spare_normal = 1;
        return mean + std_dev * z0;
    }
}

/*
 * Sample from a two-component mixture of normal distributions.
 * Picks the first component with probability weight (default 0.5)
 * and the second otherwise.
 */
float sample_mixture_of_normals(float mean1, float std_dev1,
                                float mean2, float std_dev2,
                                varargs float weight) {
    if (!weight) {
        weight = 0.5;
    }

    return random_float() < weight
        ? box_muller_normal(mean1, std_dev1)
        : box_muller_normal(mean2, std_dev2);
}

/*
 * Uniform sample in [min, max].
 */
float sample_uniform(float min, float max) {
    return min + (max - min) * random_float();
}

/*
 * Exponential distribution sample with the given rate, via inverse
 * transform sampling.
 */
float sample_exponential(float rate) {
    return -log(random_float()) / rate;
}

/*
 * Log of the acceptance ratio used by the Marsaglia-Tsang rejection
 * test: log(u) must fall below this value for a candidate sample to
 * be accepted.
 */
private float calc_acceptance_log_ratio(float x, float d, float v) {
    float x_squared_half, d_term;
    
    x_squared_half = x * x / 2.0;
    d_term = d * (1.0 - v + log(v));
    
    return x_squared_half + d_term;
}

/*
 * Gamma distribution sample via the Marsaglia-Tsang method. Only
 * valid for shape >= 1; smaller shapes are handled by
 * sample_gamma_shape_less_than_one via a boosting trick.
 */
float sample_gamma_marsaglia_tsang(float shape, float scale) {
    float d, c, x, v, u, log_ratio;
    
    d = shape - 1.0 / 3.0;
    c = 1.0 / sqrt(9.0 * d);
    
    do {
        do {
            x = box_muller_normal(0.0, 1.0);
            v = 1.0 + c * x;
        } while (v <= 0.0);
        
        v = v * v * v;
        u = random_float();
        log_ratio = calc_acceptance_log_ratio(x, d, v);
        
    } while (log(u) > log_ratio);
    
    return d * v * scale;
}

/*
 * Gamma sample for shape < 1, via the standard boosting trick:
 * sample with shape + 1 (where Marsaglia-Tsang applies) and correct
 * by a uniform variate raised to 1/shape.
 */
private float sample_gamma_shape_less_than_one(float shape, float scale) {
    float gamma_sample, u;

    gamma_sample = sample_gamma_marsaglia_tsang(shape + 1.0, scale);
    u = random_float();

    return gamma_sample * pow(u, 1.0 / shape);
}

/*
 * Gamma sample for shape == 1, which reduces to an exponential
 * distribution with rate 1/scale.
 */
private float sample_gamma_shape_equals_one(float scale) {
    return sample_exponential(1.0 / scale);
}

/*
 * Gamma distribution sample, dispatching to the appropriate method
 * based on shape: boosted Marsaglia-Tsang for shape < 1, the
 * exponential special case for shape == 1, and Marsaglia-Tsang
 * directly otherwise.
 */
float sample_gamma(float shape, float scale) {
    if (shape < 1.0) {
        return sample_gamma_shape_less_than_one(shape, scale);
    } else if (shape == 1.0) {
        return sample_gamma_shape_equals_one(scale);
    } else {
        return sample_gamma_marsaglia_tsang(shape, scale);
    }
}

/*
 * Inverse gamma distribution sample, obtained as the reciprocal of
 * a gamma sample drawn with the reciprocal scale.
 */
float sample_inverse_gamma(float shape, float scale) {
    float gamma_sample;
    
    gamma_sample = sample_gamma(shape, 1.0 / scale);
    return 1.0 / gamma_sample;
}
