# include "code.h"

inherit LPCCode;


private LPCType type;	/* class + type + *** */
private string name; 	/* name */

/*
 * initialize declaration
 */
static void create(LPCType t, string str, varargs int line)
{
    ::create(line);
    type = t;
    name = str;
}

/*
 * emit code for declaration
 */
void code(void)
{
    type->code();
    emit(name, line());
}
