#include <iostream>

#include "Serializer.h"
#include "ClientMessage.h"

using namespace std;

int main()
{
    ClientMessage c(1, 1, 1, "dfa");
    c.serialize();
    return 0;
}
