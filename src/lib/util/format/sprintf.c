#include <kfun.h>

string sprintf(string template, mixed value) {
#ifdef KF_SNPRINTF
    return snprintf(template, value);
#else
    error("snprintf() not implemented");
#endif
}
