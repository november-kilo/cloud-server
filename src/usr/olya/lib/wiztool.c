#include <NKlib.h>

inherit "~nik/lib/wiztool";

static String startIntegration(void) {
    return new String("\n" + bold("Probabilities:") + "\n");
}

static String integrationStep(String str, int f) {
    str += ralign("P(" + f + ")", 5) + " = " + new SimpsonsRuleIntegrator(new ExponentialPDF((float) f))->integrate(0., 15.) + "\n";

    return str;
}

static int displayIntegrationResults(String str, object user) {
    user->message(str->buffer()->chunk());

    return 1;
}

void cmd_integrate(object user, string cmd, string arg) {
    Continuation continuation;
    Iterator iterator;
    int from, to;

    if (!arg || sscanf(arg, "%d %d", from, to) != 2) {
        user->message("Using default values 1..20\n");
        from = 1;
        to = 20;
    }

    if (from < 1) {
        user->message("The value for 'from' should be non-zero, positive.");
    }

    continuation = new Continuation("startIntegration");
    iterator = new IntIterator(from, to);
    while (!iterator->end()) {
	continuation >>= new ChainedContinuation("integrationStep", iterator->next());
    }
    continuation >>= new ChainedContinuation("displayIntegrationResults", user);
    continuation += new Continuation("done", user);

    continuation->runNext();
}
