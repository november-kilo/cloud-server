#include "nik.h"

inherit Terminal;
inherit NK_KFUN;

private Queue queue;

static void println(object user, string str) {
    user->message(str + "\n");
}

string getPrompt(object user) {
    return magenta(user->query_name() + "> ", TRUE);
}

void cmd_colors(object user, string cmd, string str) {
    Terminal t;
    Iterator iterator;
    int *heatMap;

    heatMap = allocate_int(7);
    heatMap[0] = 46;
    heatMap[1] = 226;
    heatMap[2] = 220;
    heatMap[3] = 214;
    heatMap[4] = 208;
    heatMap[5] = 202;
    heatMap[6] = 196;
    iterator = new IntIterator(0, 6);
    t = new Terminal();

    while (!iterator->end()) {
        user->message(t->color256(heatMap[iterator->next()], "AaBbCc  "));
    }

    println(user, "Done.");
}

void cmd_de(object user, string cmd, string str) {
    object experiment;
    int t, f, k;

    if (str) {
        if (!find_object(DICE_EXPERIMENT_OBJ)) {
            compile_object(DICE_EXPERIMENT_OBJ);
        }
        experiment = clone_object(DICE_EXPERIMENT_OBJ);
        if (str == "4k3") {
            experiment->run(user, DICE_EXPR_COUNT, 4, 6, 3);
            return;
        }
        if (sscanf(str, "%dd%dk%d", t, f, k) == 3) {
            experiment->run(user, DICE_EXPR_COUNT, t, f, k);
            return;
        }
        if (sscanf(str, "%dd%d", t, f) == 2) {
            experiment->run(user, DICE_EXPR_COUNT, t, f, t);
            return;
        }
    }
    user->println("Usage: de <experiment parameters>");
}

void cmd_dex(object user, string cmd, string str) {
    float target;
    int sampleSize;
    string filename;
    object experiment;

    if (!str || !sscanf(str, "%f in %s %d", target, filename, sampleSize) == 3) {
        user->println("Usage: dex 10 in ~/data/nik-data-1-3d6k3.dice 10000");
        return;
    }

    if (!find_object(DICE_EXPERIMENT_CHECKS_OBJ)) {
        compile_object(DICE_EXPERIMENT_CHECKS_OBJ);
    }
    experiment = clone_object(DICE_EXPERIMENT_CHECKS_OBJ);
    experiment->run(user, target, sampleSize, filename);
}

static void process(mixed value, object user) {
    user->println("dequeued: " + dump_value(value, ([ ])));
}

private void initQueue() {
    if (!find_object(QUEUE_OBJ)) {
        compile_object(QUEUE_OBJ);
    }
    if (!queue) {
        queue = clone_object(QUEUE_OBJ);
    }
}

void cmd_enq(object user, string cmd, string str) {
    if (!str) {
        str = "Your significant mind notices the fabric of space.";
    }

    initQueue();
    (new DelayedContinuation(4) + queue->enqueueCont(str))->runNext();
}

void cmd_deq(object user, string cmd, string str) {
    initQueue();
    (queue->dequeueCont() >> new ChainedContinuation("process", user))->runNext();
}
