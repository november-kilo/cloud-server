# include <String.h>
# include "Record.h"

inherit KeyUpdate;


/*
 * initialize KeyUpdate from a blob
 */
static void create(String blob, int offset, int end)
{
    if (offset + 1 != end) {
	error("DECODE_ERROR");
    }
    ::create(blob[offset]);
}
