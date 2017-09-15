/* * * * * * * * * * * * *
 * Create by kinderriven *
 * 2017/9/13             *
 * * * * * * * * * * * * */

#include <stdio.h>
#include <fcntl.h>
#include <assert.h>

#define STDIN_FD        0
#define STDOUT_FD       1
#define SECTOR_SIZE     512
#define BOOT_SIZE       (SECTOR_SIZE * 1)
#define SETUP_SIZE      (SECTOR_SIZE * 4)

void write_boot(char *boot) {
    //write boot
    int fd, len;
    char buf[BOOT_SIZE];
    assert((fd = open(boot, O_RDONLY)) > 0);
    len = read(fd, buf, sizeof(buf));
    //one boot sector must be 512B
    assert(len == BOOT_SIZE);
    //write boot sector to floppy img
    len = write(STDOUT_FD, buf, BOOT_SIZE);
    assert(len == BOOT_SIZE);
    close(fd);
}

void write_setup(char *setup) {
    //write setup
    int fd, len;
    char buf[SETUP_SIZE];
    assert((fd = open(setup, O_RDONLY)) > 0);
    len = read(fd, buf, sizeof(buf));
    //one setup sector must be 2048B
    assert(len == SETUP_SIZE);
    //write setup sector to floppy img
    len = write(STDOUT_FD, buf, SETUP_SIZE);
    assert(len == SETUP_SIZE);
    close(fd);
}

int main(int argc, char **argv) {
    //check
    assert(argc > 1);
    write_boot(argv[1]);
    write_setup(argv[2]);
    return 0;
}

