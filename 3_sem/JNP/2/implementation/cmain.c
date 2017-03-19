#include "stdio.h" 
#include "strdeque.h"

int main()
{
    printf("%ld\n", strdeque_new());
    strdeque_insert_at(1, 0, "three");
    strdeque_insert_at(1, 0, "two");
    strdeque_insert_at(1, 0, "one");
    printf("%ld\n", strdeque_size(1));
    strdeque_clear(1);
    printf("%ld\n", strdeque_size(1));
    return 0;
}
