#include <uuid/uuid.h>
#include "lpc_ext.h"

#define KFUN_COUNT 1

static void lower_case(char *buffer) {
    for (int i = 0; i < 36; i++) {
        if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
            buffer[i] |= 0x60;
        }
    }
}

static void set_return_value(LPC_frame lpc_frame, char* output, LPC_value return_value) {
    LPC_dataspace lpc_dataspace;
    LPC_string lpc_string;

    lpc_dataspace = lpc_frame_dataspace(lpc_frame);
    lpc_string = lpc_string_new(lpc_dataspace, output, 36);
    lpc_string_putval(return_value, lpc_string);
}

static void kf_uuid(LPC_frame lpc_frame, int arg_count, LPC_value return_value) {
    char output[40];
    uuid_t uuid;

    uuid_generate(uuid);
    uuid_unparse(uuid, output);
    lower_case(output);
    set_return_value(lpc_frame, output, return_value);
}

static char kf_uuid_proto[] = {LPC_TYPE_STRING, LPC_TYPE_VOID, 0};
static LPC_ext_kfun kf[KFUN_COUNT] = {
        {"uuid", kf_uuid_proto, &kf_uuid}
};

int lpc_ext_init(int major, int minor, const char *config) {
    lpc_ext_kfun(kf, KFUN_COUNT);
    return 1;
}
