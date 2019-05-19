#include <NKlib.h>

int randomInt(varargs int p, int q) {
    if (p == 0 && q == 0) {
        return random(0);
    }

    return random(0) % (q - p + 1) + p;
}

float randomFloat(varargs float p, float q) {
    if (p == 0.  && q == 0.) {
        return (float) random(0);
    }

    return ((float) random(0) / (float) INT_MAX) * (q - p) + p;
}

string randomString(int length) {
    int x;
    string str;
    string *results;
    Iterator iterator;

    if (length < 1) {
        error("Random: cannot generate a random string with size less than unity.");
    }

    str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    iterator = new IntIterator(0, length - 1);
    results = allocate(length);

    while (!iterator->end()) {
        x = random(strlen(str));
        results[iterator->next()] = str[x..x];
    }

    return implode(results, "");
}
