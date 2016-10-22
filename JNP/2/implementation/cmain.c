#include "stdio.h" 
#include "strdeque.h"

int main()
{
    printf("%ld\n", strdeque_new());
    strdeque_insert_at(0, 0, "three");
    strdeque_insert_at(0, 0, "two");
    strdeque_insert_at(0, 0, "one");
    printf("%ld\n", strdeque_size(0));
    strdeque_clear(0);
    printf("%ld\n", strdeque_size(0));
    return 0;
}
