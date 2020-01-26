#include "string_kfuns.h"

static void kf_replace(LPC_frame frame, int nargs, LPC_value returnValue) {
	LPC_dataspace data;
	LPC_array inputArray, outputArray;
	LPC_string replaceThis, withThat;
	LPC_int maxArray, maxString;
	LPC_value value;

	int i, x, blocks, extraBlock;
	unsigned long joinedLen;
	sds string, a, b, joined;
	sds *tokens;

	data = lpc_frame_dataspace(frame);
	inputArray = lpc_array_getval(lpc_frame_arg(frame, nargs, 0));
	replaceThis = lpc_string_getval(lpc_frame_arg(frame, nargs, 1));
	withThat = lpc_string_getval(lpc_frame_arg(frame, nargs, 2));
	maxArray = lpc_int_getval(lpc_frame_arg(frame, nargs, 3));
	maxString = lpc_int_getval(lpc_frame_arg(frame, nargs, 4));

	string = lpcArrayToSds(inputArray);
	a = sdsnew(lpc_string_text(replaceThis));
	b = sdsnew(lpc_string_text(withThat));
	tokens = sdssplitlen(string, sdslen(string), a, sdslen(a), &x);
	joined = sdsjoinsds(tokens, x, b, sdslen(b));

	joinedLen = sdslen(joined);
	blocks = joinedLen / maxString;
	if (blocks == 0) {
		extraBlock = 1;
	} else {
		extraBlock = (fmod(joinedLen, maxString) != 0.0);
	}

	if (blocks + extraBlock > maxArray) {
		lpc_runtime_error(frame, "replace: output array too large");
	}

	outputArray = lpc_array_new(data, blocks + extraBlock);
	value = lpc_value_temp(data);

	for (i = 0; i < blocks; i++) {
		char *copy;

		copy = copyBlock(joined, maxString);
		lpc_string_putval(value, lpc_string_new(data, copy, strlen(copy)));
		lpc_runtime_ticks(frame, strlen(copy));
		free(copy);
		lpc_array_assign(data, outputArray, i, value);
		sdsrange(joined, maxString, -1);
	}

	if (extraBlock) {
		lpc_string_putval(value, lpc_string_new(data, joined, strlen(joined)));
		lpc_runtime_ticks(frame, strlen(joined));
		lpc_array_assign(data, outputArray, i, value);
	}

	sdsfree(string);
	sdsfree(a);
	sdsfree(b);
	sdsfreesplitres(tokens, x);
	sdsfree(joined);

	lpc_array_putval(returnValue, outputArray);
}

static char kf_replace_proto[] = {
		LPC_TYPE_ARRAY_OF(LPC_TYPE_STRING),
		LPC_TYPE_ARRAY_OF(LPC_TYPE_STRING),
		LPC_TYPE_STRING, LPC_TYPE_STRING,
		LPC_TYPE_INT, LPC_TYPE_INT, 0
};

static LPC_ext_kfun kf[STRING_KFUNS_COUNT] = {
		{"replace", kf_replace_proto, &kf_replace}
};

int lpc_ext_init(int major, int minor, const char *config) {
	lpc_ext_kfun(kf, STRING_KFUNS_COUNT);

	return 1;
}

