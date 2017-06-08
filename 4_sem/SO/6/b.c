#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/hello", O_RDONLY);
    char dt[100];
    read(fd, dt, 100);
    for (int i = 0; i < 100; i++)
    {
        printf("%c %d\n", dt[i], (int)(dt[i]));
    }
}
