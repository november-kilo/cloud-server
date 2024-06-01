#include <Sort.h>

private void merge(mixed *array, int l, int m, int r) {
    int i, j, k;
    int n1, n2;
    mixed *left, *right;

    n1 = m - l + 1;
    n2 = r - m;
    left = allocate(n1);
    right = allocate(n2);

    for (i = 0; i < n1; i++) {
        left[i] = array[l + i];
    }

    for (j = 0; j < n2; j++) {
        right[j] = array[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            array[k] = left[i];
            i++;
        } else {
            array[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = right[j];
        j++;
        k++;
    }
}

private void merge_sort(mixed *array, int l, int r) {
    int m;

    if (l < r) {
        m = l + (r - l) / 2;
        merge_sort(array, l, m);
        merge_sort(array, m + 1, r);
        merge(array, l, m, r);
    }
}

void sort(mixed *array, varargs int l, int r) {
    if (!l) {
        l = 0;
    }
    if (!r) {
        r = sizeof(array) - 1;
    }
    merge_sort(array, l, r);
}
