#include <math.h>
#include "lpc_ext.h"

#define KFUN_COUNT 2

static void kf_log1p(LPC_frame lpc_frame, int arg_count, LPC_value return_value) {
    lpc_float_putval(return_value, log1pl(lpc_float_getval(lpc_frame_arg(lpc_frame, arg_count, 0))));
}

static void kf_hypot(LPC_frame lpc_frame, int arg_count, LPC_value return_value) {
    LPC_float x, y;

    x = lpc_float_getval(lpc_frame_arg(lpc_frame, arg_count, 0));
    y = lpc_float_getval(lpc_frame_arg(lpc_frame, arg_count, 1));
    lpc_float_putval(return_value, hypotl(x, y));
}

static char kf_log1p_proto[] = {
        LPC_TYPE_FLOAT, LPC_TYPE_FLOAT, 0
};

static char kf_hypot_proto[] = {
        LPC_TYPE_FLOAT, LPC_TYPE_FLOAT, LPC_TYPE_FLOAT, 0
};

static LPC_ext_kfun kf[KFUN_COUNT] = {
        {"log1p", kf_log1p_proto, &kf_log1p},
        {"hypot", kf_hypot_proto, &kf_hypot}
};

int lpc_ext_init(int major, int minor, const char *config) {
    lpc_ext_kfun(kf, KFUN_COUNT);

    return 1;
}
