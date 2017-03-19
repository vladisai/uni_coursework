#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "strdeque.h"
#include "strdequeconst.h"

int streq(const char* s1, const char* s2, size_t len) {
    if (s1 == s2)
        return 1;

    if (s1 == NULL || s2 == NULL)
        return 0;

    return strncmp(s1, s2, len) == 0;
}

int main() {
    unsigned long d1, d2, d3;

    d1 = strdeque_new();

    strdeque_insert_at(d1, 0, "a");
    assert(streq(strdeque_get_at(d1, 0), "a", 2));

    strdeque_insert_at(d1, 8, "aa");
    assert(streq(strdeque_get_at(d1, 1), "aa", 3));

    strdeque_remove_at(d1, 1);
    assert(strdeque_get_at(d1, 1) == NULL);
    assert(streq(strdeque_get_at(d1, 0), "a", 2));

    strdeque_insert_at(d1, 0, NULL);
    assert(streq(strdeque_get_at(d1, 0), "a", 2));
    assert(strdeque_size(d1) == 1);

    strdeque_remove_at(d1, 1);
    assert(strdeque_size(d1) == 1);

    strdeque_delete(d1);
    strdeque_insert_at(d1, 0, "b");
    assert(strdeque_size(d1) == 0);
    assert(strdeque_get_at(d1, 0) == NULL);

    d2 = strdeque_new();
    d3 = strdeque_new();
    strdeque_insert_at(d2, 0, "c");
    strdeque_insert_at(d2, 0, "cc");
    strdeque_insert_at(d2, 0, "ccc");
    strdeque_insert_at(d3, 100, "ccc");
    strdeque_insert_at(d3, 1000, "cc");
    strdeque_insert_at(d3, 10000, "c");
    assert(strdeque_comp(d2, d3) == 0);

    strdeque_remove_at(d2, 0);
    assert(strdeque_comp(d2, d3) == -1);

    strdeque_delete(d3);
    assert(strdeque_comp(d2, d3) == 1);

    strdeque_delete(d2);
    assert(strdeque_comp(d2, d3) == 0);
    assert(strdeque_comp(d2, emptystrdeque()) == 0);
    assert(strdeque_comp(emptystrdeque(), d3) == 0);
    assert(strdeque_comp(emptystrdeque(), emptystrdeque()) == 0);

    assert(strdeque_size(emptystrdeque()) == 0);

    strdeque_delete(emptystrdeque());
    strdeque_insert_at(emptystrdeque(), 0, "d");
    assert(strdeque_size(emptystrdeque()) == 0);

    strdeque_insert_at(emptystrdeque(), 0, "d");
    strdeque_insert_at(emptystrdeque(), 0, "dd");
    assert(strdeque_get_at(emptystrdeque(), 0) == NULL);

    strdeque_remove_at(emptystrdeque(), 0);
    assert(strdeque_size(emptystrdeque()) == 0);

    strdeque_clear(emptystrdeque());

    return 0;
}
