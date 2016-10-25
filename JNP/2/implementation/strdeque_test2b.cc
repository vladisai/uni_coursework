#include "cstrdequeconst"
#include <cassert>

namespace {
    unsigned long id = ::jnp1::emptystrdeque();
}

int main() {
    assert(id == 0);
}
