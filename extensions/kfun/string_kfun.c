#include <stdio.h>
#include "lpc_ext.h"

static void kf_snprintf(LPC_frame frame, int nargs, LPC_value retval) {
    LPC_dataspace data;
    LPC_value val;
    char buf[65535];
    char *format;
    int i;

    format = lpc_string_text(lpc_string_getval(lpc_frame_arg(frame, nargs, 0)));
    val = lpc_frame_arg(frame, nargs, 1);
    switch (lpc_value_type(val)) {
        case LPC_TYPE_STRING:
            i = snprintf(buf, 65535, format, lpc_string_text(lpc_string_getval(val)));
            break;
        case LPC_TYPE_INT:
            i = snprintf(buf, 65535, format, lpc_int_getval(val));
            break;
        case LPC_TYPE_FLOAT:
            i = snprintf(buf, 65535, format, lpc_float_getval(val));
            break;
        default:
            i = -1;
    }

    if (i < 0) {
        lpc_runtime_error(frame, "Invalid template");
    } else {
        lpc_string_putval(retval, lpc_string_new(lpc_frame_dataspace(frame), buf, i));
    }
}

static char kf_snprintf_proto[] = {LPC_TYPE_STRING, LPC_TYPE_STRING, LPC_TYPE_MIXED, 0};
static LPC_ext_kfun kf[1] = {
        {"snprintf", kf_snprintf_proto, &kf_snprintf}
};

int lpc_ext_init(int major, int minor, const char *config) {
    lpc_ext_kfun(kf, 1);
    return 1;
}
