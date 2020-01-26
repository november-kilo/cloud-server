#include <Array.h>

inherit ArrayToListReducer;

#define LIST_GAP " "

private int order;

static string ifNotNextValue(mixed currentValue, varargs int isLast) {
	return (order > 0 ? (order++) + ")" : pointer()) + LIST_GAP + currentValue + "\n";
}

static string ifNextValue(mixed nextValue, mixed currentValue, varargs int isLast) {
	return currentValue +
			(order > 0 ? (order++) + ")" : pointer()) +
			LIST_GAP +
			nextValue + "\n";
}

void apply(int order) {
    ::order = order;
}

static void create(varargs int order) {
    ::order = order;
}