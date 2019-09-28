#ifndef PERL_KFUNS_H

#define PERL_KFUNS_H

#include <EXTERN.h>
#include <perl.h>
#include "lpc_ext.h"

#define PERL_KFUNS_COUNT 3

#define PERL_KFUN_SETUP my_perl = perl_alloc();\
perl_construct(my_perl);\
perl_parse(my_perl, NULL, 3, embedding, NULL);\
PL_exit_flags |= PERL_EXIT_DESTRUCT_END

#define PERL_KFUN_CLEANUP PL_perl_destruct_level = 1;\
perl_destruct(my_perl);\
perl_free(my_perl)

#endif
