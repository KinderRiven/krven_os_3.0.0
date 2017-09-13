/* * * * * * * * * * * * *
 * Create by kinderriven *
 * 2017/9/13             *
 * * * * * * * * * * * * */

#include <stdio.h>
#include <fcntl.h>
#include <assert.h>

#define STDIN_FD  0
#define STDOUT_FD 1
#define BUFFER_SIZE 1024
#define SECTOR_SIZE 512

int main(int argc, char **argv) {
    int fd, len;
    char buf[BUFFER_SIZE];
    //check
    assert(argc > 1);
    assert((fd = open(argv[1], O_RDONLY)) > 0);
    len = read(fd, buf, sizeof(buf));
    //one boot sector must be 512B
    assert(len == SECTOR_SIZE);
    //write boot sector to floppy img
    len = write(STDOUT_FD, buf, SECTOR_SIZE);
    assert(len == SECTOR_SIZE);
    return 0;
}

