#include <uuid/uuid.h>
#include "lpc_ext.h"

static void kf_uuid(LPC_frame f, int nargs, LPC_value retval) {
    LPC_dataspace data;
    LPC_string str;
    char buf[40];
    uuid_t uu;
    int i;

    uuid_generate(uu);
    uuid_unparse(uu, buf);
    for (i = 0; i < 36; i++) {
        if (buf[i] >= 'A' && buf[i] <= 'Z') {
            buf[i] |= 0x60;
        }
    }

    data = lpc_frame_dataspace(f);
    str = lpc_string_new(data, buf, i);
    lpc_string_putval(retval, str);
}

static char kf_uuid_proto[] = {LPC_TYPE_STRING, LPC_TYPE_VOID, 0};
static LPC_ext_kfun kf[1] = {
        {"uuid", kf_uuid_proto, &kf_uuid}
};

int lpc_ext_init(int major, int minor, const char *config) {
    lpc_ext_kfun(kf, 1);
    return 1;
}
