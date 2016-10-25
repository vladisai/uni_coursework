#include <cassert>
#include <cstddef>
#include <cstring>
#include "cstrdeque"

int streq(const char* s1, const char* s2, size_t len) {
    if (s1 == s2)
        return 1;

    if (s1 == NULL || s2 == NULL)
        return 0;

    return strncmp(s1, s2, len) == 0;
}

namespace {
    unsigned long testuj() {
        unsigned long id = ::jnp1::strdeque_new();
        ::jnp1::strdeque_insert_at(id, 0, "");
        return id;
    }

    unsigned long id = testuj();
}

int main() {
    const char* str = ::jnp1::strdeque_get_at(id, 0);
    assert(streq(str, "", 1));
    ::jnp1::strdeque_delete(id);
}
