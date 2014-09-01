# include "code.h"

inherit Code;


private string classType;	/* class + type */
private Expression objType;	/* object type */
private int indirection;	/* *** */

/*
 * NAME:	create()
 * DESCRIPTION:	initialize declaration
 */
static void create(string typeStr, Expression typeName, int ind,
		   varargs int line)
{
    ::create(line);
    classType = typeStr;
    objType = typeName;
    indirection = ind;
}

/*
 * NAME:	code()
 * DESCRIPTION:	emit code for declaration
 */
void code()
{
    emit(classType + " ", line());
    if (objType) {
	objType->code();
    }
    if (indirection) {
	emit("***************"[.. indirection - 1]);
    }
}