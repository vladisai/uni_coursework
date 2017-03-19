extern "C" {
#include "strdequeconst.h"
}
#include <iostream>

extern "C" {

const int EMPTY_ID = 0;

size_t emptystrdeque()
{
#ifdef NDEBUG
    std::cerr << "emptystrdeque()" << std::endl;
#endif
    return EMPTY_ID;
}
}
