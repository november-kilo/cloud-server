#include <stdio.h>
#include "lpc_ext.h"

static void discord_read(LPC_frame f, int nargs, LPC_value retval) {
    LPC_dataspace data;
    LPC_string str;
    char buf[8001];
    int i;

    i = lpc_ext_read(buf, 8001);
    data = lpc_frame_dataspace(f);
    str = lpc_string_new(data, buf, i);
    lpc_string_putval(retval, str);
}

static void discord_write(LPC_frame f, int nargs, LPC_value retval) {
    LPC_string str;
    int written;
    str = lpc_string_getval(lpc_frame_arg(f, nargs, 0));
    written = lpc_ext_write(lpc_string_text(str), lpc_string_length(str));
    lpc_int_putval(retval, written);
}

static char discord_read_proto[] = {LPC_TYPE_STRING, LPC_TYPE_VOID, 0};
static char discord_write_proto[] = {LPC_TYPE_INT, LPC_TYPE_STRING, 0};
static LPC_ext_kfun kf[2] = {
        {"discord_read", discord_read_proto, &discord_read},
        {"discord_write", discord_write_proto, &discord_write}
};

int lpc_ext_init(int major, int minor, const char *config) {
    char cmd[1000];
    snprintf(cmd, 1000, "exec %s/bin/discord_bot", config);
    lpc_ext_kfun(kf, 2);
    return lpc_ext_spawn(cmd);
}
