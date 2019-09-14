#include <EXTERN.h>
#include <perl.h>
#include "lpc_ext.h"

#define PERL_KFUN_COUNT 3

#define PERL_KFUN_SETUP my_perl = perl_alloc();\
perl_construct(my_perl);\
perl_parse(my_perl, NULL, 3, embedding, NULL);\
PL_exit_flags |= PERL_EXIT_DESTRUCT_END

#define PERL_KFUN_CLEANUP PL_perl_destruct_level = 1;\
perl_destruct(my_perl);\
perl_free(my_perl)

static PerlInterpreter *my_perl;

static SV* my_eval_sv(SV *sv, I32 croak_on_error, LPC_frame lpcFrame)
{
    dSP;
    SV* retval;

    PUSHMARK(SP);
    eval_sv(sv, G_SCALAR);
    SPAGAIN;
    retval = POPs;
    PUTBACK;

    if (croak_on_error && SvTRUE(ERRSV)) {
        lpc_runtime_error(lpcFrame, SvPVx_nolen(ERRSV));
    }

    return retval;
}

static SSize_t match(SV *string, char *pattern, AV **match_list, LPC_frame lpcFrame)
{
    SV *command = newSV(0);
    SSize_t num_matches;
    sv_setpvf(command, "$string = '%s'; @array = ($string =~ %s)", SvPV_nolen(string), pattern);
    my_eval_sv(command, TRUE, lpcFrame);
    SvREFCNT_dec(command);
    *match_list = get_av("array", 0);
    num_matches = av_top_index(*match_list) + 1;
    return num_matches;
}

static I32 substitute(SV **string, char *pattern, LPC_frame lpcFrame) {
    SV *command = newSV(0), *retval;
    sv_setpvf(command, "$string = '%s'; ($string =~ %s)", SvPV_nolen(*string), pattern);
    retval = my_eval_sv(command, TRUE, lpcFrame);
    SvREFCNT_dec(command);

    *string = get_sv("string", 0);

    return SvIV(retval);
}

static void perl_match(LPC_frame frame, int nargs, LPC_value returnValue) {
    char *embedding[] = { "", "-e", "0", NULL };
    AV *match_list;
    I32 num_matches, i, j;
    SV *text;
    SV *m;
    char *out[1000];

    LPC_dataspace data;
    LPC_value value;
    LPC_string searchIn, searchFor, lpcMatch;
    LPC_array results;

    searchIn = lpc_string_getval(lpc_frame_arg(frame, nargs, 0));
    searchFor = lpc_string_getval(lpc_frame_arg(frame, nargs, 1));

    PERL_KFUN_SETUP;
    text = newSV(0);
    sv_setpv(text, lpc_string_text(searchIn));

    num_matches = match(text, lpc_string_text(searchFor), &match_list, frame);
    lpc_int_putval(returnValue, num_matches);

    if (num_matches > 0) {
        for (i = 0; i < num_matches; i++) {
            m = *av_fetch(match_list, i, FALSE);
            if (m != NULL) {
                out[i] = SvPV_nolen(m);
            }
        }

        data = lpc_frame_dataspace(frame);
        value = lpc_value_temp(data);
        results = lpc_array_new(data, i);

        for (j = 0; j < i; j++) {
            lpcMatch = lpc_string_new(data, out[j], strlen(out[j]));
            lpc_string_putval(value, lpcMatch);
            lpc_array_assign(data, results, j, value);
        }

        lpc_array_putval(returnValue, results);
    }

    SvREFCNT_dec(text);
    PERL_KFUN_CLEANUP;
}

static void perl_sub(LPC_frame frame, int nargs, LPC_value returnValue) {
    char *embedding[] = { "", "-e", "0", NULL };
    I32 num_matches;
    SV *text;
    char *p;

    LPC_dataspace data;
    LPC_value value;
    LPC_string searchIn, searchFor, lpcString;

    searchIn = lpc_string_getval(lpc_frame_arg(frame, nargs, 0));
    searchFor = lpc_string_getval(lpc_frame_arg(frame, nargs, 1));

    PERL_KFUN_SETUP;
    text = newSV(0);
    sv_setpv(text, lpc_string_text(searchIn));

    num_matches = substitute(&text, lpc_string_text(searchFor), frame);
    if (num_matches > 0) {
        p = SvPV_nolen(text);
        data = lpc_frame_dataspace(frame);
        value = lpc_value_temp(data);
        lpcString = lpc_string_new(data, p, strlen(p));

        lpc_string_putval(returnValue, lpcString);
    }

    SvREFCNT_dec(text);
    PERL_KFUN_CLEANUP;
}

static void perl_term(LPC_frame frame, int nargs, LPC_value returnValue) {
    PERL_SYS_TERM();
}

static char perl_match_proto[] = {
        LPC_TYPE_ARRAY_OF(LPC_TYPE_STRING),
        LPC_TYPE_STRING,
        LPC_TYPE_STRING,
        0
};

static char perl_sub_proto[] = {
        LPC_TYPE_STRING,
        LPC_TYPE_STRING,
        LPC_TYPE_STRING,
        0
};

static char perl_term_proto[] = {
        LPC_TYPE_VOID, 0
};

static LPC_ext_kfun kf[PERL_KFUN_COUNT] = {
    { "perl_match", perl_match_proto, &perl_match },
    { "perl_sub", perl_sub_proto, &perl_sub },
    { "perl_term", perl_term_proto, &perl_term }
};

int lpc_ext_init(int major, int minor, const char *config) {
    char *argvv[] = { NULL, NULL };
    char **argv = argvv;
    int argc = 0;
    argv[argc++] = "";
    char **env;
    PERL_SYS_INIT3(&argc,&argv,&env);

    lpc_ext_kfun(kf, PERL_KFUN_COUNT);

    return 1;
}
