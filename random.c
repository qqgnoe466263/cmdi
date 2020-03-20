#include "random.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int random_number()
{
    int number = 0;
    static int fd = 0;
    fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "[!] Open /dev/urandom failed !!");
        exit(0);
    }

    int r = read(fd, &number, 4);
    if (read(fd, &number, 4) < 0) {
        fprintf(stderr, "[!] Read /dev/urandom failed !!");
        exit(0);
    }

    return number;
}
