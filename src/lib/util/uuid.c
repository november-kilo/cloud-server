#include <kfun.h>

string uuid(varargs int strip_dashes) {
#ifndef KF_UUID
    error("uuid() not implemented");
#else
    string id;

    id = ::uuid();

    if (strip_dashes) {
        return implode(explode(id, "-"), "");
    }

    return id;
#endif
}
