#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "err.h"

#define N 5
#define stdout_dsc 1

char message[] = "Hello from your child!";

int main (int argc, char *argv[])
{
  int i;
  for (i = 0; i < N; i++) {
    sleep(3);
    if (write(stdout_dsc, message, sizeof(message)) != sizeof(message))
      syserr("Error in write\n");
  }  
  exit(0);
}
