#include "string_utils.h"

sds lpcArrayToSds(LPC_array array) {
	sds string;
	int i, n;

	string = sdsempty();
	n = lpc_array_size(array);

	for (i = 0; i < n; i++) {
		string = sdscat(string, lpc_string_text(lpc_string_getval(lpc_array_index(array, i))));
	}

	return string;
}

char *copyBlock(sds string, int n) {
	char *copy;

	copy = malloc((n + 1) * sizeof(char));
	memset(copy, '\0', n + 1);
	strncpy(copy, string, n);

	return copy;
}
