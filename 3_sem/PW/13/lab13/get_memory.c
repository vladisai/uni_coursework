#include <fcntl.h> /* For O_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "err.h"

#define SHM_NAME "/pw_practice_memory"
#define NAP_TIME 2

int main() {

    int fd_memory = -1; /* deskryptor dla pamięci*/

    printf("Obecnie w /dev/shm znajdują się:\n");
    if (system("ls /dev/shm")) /*wykonaj komendę systemową*/
        syserr("ls");
    printf("\n\n");

    fd_memory = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd_memory == -1)
        syserr("shm_open");

    printf("Stworzyłem fragment pamięci, jeśli nie istniał\n");
    printf("Obecnie w /dev/shm znajdują się:\n");
    if (system("ls /dev/shm")) /*wykonaj komendę systemową*/
        syserr("ls");
    printf("\n\n");

    sleep(NAP_TIME);

    close(fd_memory); /*plik specjalny dalej istnieje*/

    printf("Zamknąłem deskryptor pamięci\n");
    printf("Obecnie w /dev/shm znajdują się:\n");
    if (system("ls /dev/shm")) /*wykonaj komendę systemową*/
        syserr("ls");
    printf("\n\n");

    sleep(NAP_TIME);

    shm_unlink(SHM_NAME); /*usunęliśmy plik specjalny*/

    printf("Usunąłem plik pamięci\n");
    printf("Obecnie w /dev/shm znajdują się:\n");
    if (system("ls /dev/shm")) /*wykonaj komendę systemową*/
        syserr("ls");
    printf("\n\n");

    return 0;
}
