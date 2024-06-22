#include <stdio.h>
#include "lpc_ext.h"

#define BUFFER_SIZE 65535
#define KFUN_COUNT 1

static int format_output(char* formatted_output, char* format, LPC_value lpc_value) {
    switch (lpc_value_type(lpc_value)) {
        case LPC_TYPE_STRING:
            return snprintf(formatted_output, BUFFER_SIZE, format, lpc_string_text(lpc_string_getval(lpc_value)));
        case LPC_TYPE_INT:
            return snprintf(formatted_output, BUFFER_SIZE, format, lpc_int_getval(lpc_value));
        case LPC_TYPE_FLOAT:
            return snprintf(formatted_output, BUFFER_SIZE, format, lpc_float_getval(lpc_value));
        default:
            return -1;
    }
}

static void set_return_value(LPC_frame lpc_frame, char* formatted_output, int characters_written, LPC_value return_value) {
    if (characters_written < 0) {
        lpc_runtime_error(lpc_frame, "Invalid template");
    } else {
        lpc_string_putval(return_value, lpc_string_new(lpc_frame_dataspace(lpc_frame), formatted_output, characters_written));
    }
}

static void kf_snprintf(LPC_frame lpc_frame, int arg_count, LPC_value return_value) {
    LPC_value lpc_value;
    char formatted_output[BUFFER_SIZE];
    char* format;
    int characters_written;

    format = lpc_string_text(lpc_string_getval(lpc_frame_arg(lpc_frame, arg_count, 0)));
    lpc_value = lpc_frame_arg(lpc_frame, arg_count, 1);

    characters_written = format_output(formatted_output, format, lpc_value);
    set_return_value(lpc_frame, formatted_output, characters_written, return_value);
}

static char kf_snprintf_proto[] = {LPC_TYPE_STRING, LPC_TYPE_STRING, LPC_TYPE_MIXED, 0};
static LPC_ext_kfun kf[KFUN_COUNT] = {
        {"snprintf", kf_snprintf_proto, &kf_snprintf}
};

int lpc_ext_init(int major, int minor, const char *config) {
    lpc_ext_kfun(kf, KFUN_COUNT);
    return 1;
}
