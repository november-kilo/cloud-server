#include "math_kfuns.h"

static void kf_log1p(LPC_frame frame, int nargs, LPC_value returnValue) {
	lpc_float_putval(returnValue, log1pl(lpc_float_getval(lpc_frame_arg(frame, nargs, 0))));
}

static char kf_log1p_proto[] = {
		LPC_TYPE_FLOAT, LPC_TYPE_FLOAT, 0
};

static LPC_ext_kfun kf[MATH_KFUNS_COUNT] = {
		{"log1p", kf_log1p_proto, &kf_log1p}
};

int lpc_ext_init(int major, int minor, const char *config) {
	lpc_ext_kfun(kf, MATH_KFUNS_COUNT);

	return 1;
}
