#include "metrics/metrics_common.h"
#include "metrics/metrics_status.h"
#include "metrics/metrics_error.h"

#define KFUN_COUNT 3

static char kf_record_status_metrics_proto[] = {LPC_TYPE_VOID, LPC_TYPE_MAPPING, 0};
static char kf_record_error_metrics_proto[] = {LPC_TYPE_VOID, LPC_TYPE_MAPPING, 0};
static char kf_cleanup_old_metrics_proto[] = {LPC_TYPE_VOID, LPC_TYPE_INT, 0};

static void kf_record_status_metrics(LPC_frame frame, int arg_count, LPC_value return_value) {
    if (arg_count < 1) {
        lpc_runtime_error(frame, "Expected status metrics mapping argument");
        return;
    }

    LPC_value value = lpc_frame_arg(frame, arg_count, 0);
    LPC_mapping metrics = lpc_mapping_getval(value);

    StatusMetrics m;
    collect_status_metrics(frame, metrics, &m);
    save_status_metrics(frame, &m);
}

static void kf_record_error_metrics(LPC_frame frame, int arg_count, LPC_value return_value) {
    if (arg_count < 1) {
        lpc_runtime_error(frame, "Expected error metrics mapping argument");
        return;
    }

    LPC_value value = lpc_frame_arg(frame, arg_count, 0);
    LPC_mapping metrics = lpc_mapping_getval(value);

    ErrorMetrics m;
    collect_error_metrics(frame, metrics, &m);
    save_error_metrics(frame, &m);
}

static void kf_cleanup_old_metrics(LPC_frame frame, int arg_count, LPC_value return_value) {
    if (arg_count < 1) {
        lpc_runtime_error(frame, "Expected days argument");
        return;
    }

    LPC_value value = lpc_frame_arg(frame, arg_count, 0);
    int days = lpc_int_getval(value);

    if (days <= 0) {
        lpc_runtime_error(frame, "Days must be positive");
        return;
    }

    cleanup_old_metrics(frame, days);
}

static LPC_ext_kfun kf[KFUN_COUNT] = {
    {"record_status_metrics", kf_record_status_metrics_proto, &kf_record_status_metrics},
    {"record_error_metrics", kf_record_error_metrics_proto, &kf_record_error_metrics},
    {"cleanup_old_metrics", kf_cleanup_old_metrics_proto, &kf_cleanup_old_metrics}
};

int lpc_ext_init(int major, int minor, const char *config) {
    lpc_ext_kfun(kf, KFUN_COUNT);
    return init_database();
}
