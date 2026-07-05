#include <statistics_lib.h>
#include "lpc_ext.h"

#include <stdio.h>

#define KFUN_COUNT 2

static void kf_statistics(LPC_frame lpc_frame, int arg_count, LPC_value return_value) {
    LPC_dataspace dataspace;
    LPC_array inputs, outputs;
    LPC_float *data;
    LPC_float d;
    LPC_value value;
    LPC_string lpc_string;
    int i, size;
    StatsReport *stats_report;
    char *stats_string, *analysis_string, *distribution_string, *outliers_string;

    dataspace = lpc_frame_dataspace(lpc_frame);
    value = lpc_frame_arg(lpc_frame, arg_count, 0);
    inputs = lpc_array_getval(value);
    size = lpc_array_size(inputs);
    data = (LPC_float *)malloc(size * sizeof(LPC_float));
    for (i = 0; i < size; i++) {
        value = lpc_array_index(inputs, i);
        d = lpc_float_getval(value);
        data[i] = d;
    }

    stats_report = compute_statisticsl(data, size);
    stats_string = format_histogram(stats_report);
    printf("histogram:\n%s\n", stats_string);
    analysis_string = format_analysis(stats_report);
    printf("analysis:\n%s\n", analysis_string);
    distribution_string = format_distribution(stats_report);
    printf("distribution:\n%s\n", distribution_string);
    outliers_string = format_outliers(stats_report);
    printf("outliers:\n%s\n", outliers_string);

    outputs = lpc_array_new(dataspace, 4);

    lpc_string = lpc_string_new(dataspace, stats_string, strlen(stats_string));
    lpc_string_putval(value, lpc_string);
    lpc_array_assign(dataspace, outputs, 0, value);

    lpc_string = lpc_string_new(dataspace, analysis_string, strlen(analysis_string));
    lpc_string_putval(value, lpc_string);
    lpc_array_assign(dataspace, outputs, 1, value);

    lpc_string = lpc_string_new(dataspace, distribution_string, strlen(distribution_string));
    lpc_string_putval(value, lpc_string);
    lpc_array_assign(dataspace, outputs, 2, value);

    lpc_string = lpc_string_new(dataspace, outliers_string, strlen(outliers_string));
    lpc_string_putval(value, lpc_string);
    lpc_array_assign(dataspace, outputs, 3, value);

    lpc_array_putval(return_value, outputs);

    free_statistics_report(stats_report);
}

static void kf_statistics_compare(LPC_frame lpc_frame, int arg_count, LPC_value return_value) {
    
}

static char kf_statistics_proto[] = {
    LPC_TYPE_ARRAY_OF(LPC_TYPE_STRING),
    LPC_TYPE_ARRAY_OF(LPC_TYPE_FLOAT),
    0
};

static char kf_statistics_compare_proto[] = {
    LPC_TYPE_STRING,
    LPC_TYPE_ARRAY_OF(LPC_TYPE_FLOAT),
    LPC_TYPE_ARRAY_OF(LPC_TYPE_FLOAT),
    0
};

static LPC_ext_kfun kf[KFUN_COUNT] = {
        {"statistics", kf_statistics_proto, &kf_statistics},
        {"statistics_compare", kf_statistics_compare_proto, &kf_statistics_compare}
};

int lpc_ext_init(int major, int minor, const char *config) {
    lpc_ext_kfun(kf, KFUN_COUNT);

    return 1;
}
