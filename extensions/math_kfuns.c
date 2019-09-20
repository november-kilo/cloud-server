#include <math.h>
#include "lpc_ext.h"

#define MATH_KFUN_COUNT 1

static void log1p_kfun(LPC_frame frame, int nargs, LPC_value returnValue) {
    lpc_float_putval(returnValue, log1pl(lpc_float_getval(lpc_frame_arg(frame, nargs, 0))));
}

static char log1p_kfun_proto[] = {
        LPC_TYPE_FLOAT, LPC_TYPE_FLOAT, 0
};

static LPC_ext_kfun kf[MATH_KFUN_COUNT] = {
        { "log1p", log1p_kfun_proto, &log1p_kfun }
};

int lpc_ext_init(int major, int minor, const char *config) {
    lpc_ext_kfun(kf, MATH_KFUN_COUNT);

    return 1;
}
