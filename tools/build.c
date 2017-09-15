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
#define BOOT_SIZE       (SECTOR * 1)
#define SETUP_SIZE      (SECTOR * 4)

void write_boot(char *boot) {
    //write boot
    int fd, len;
<<<<<<< HEAD
    char buf[BOOT_SIZE];
    assert((fd = open(boot, O_RDONLY)) > 0);
=======
    char buf[BUFFER_SIZE];
    //check
    assert(argc > 1);
    assert((fd = open(argv[1], O_RDONLY, 0)) > 0);
>>>>>>> 4776a2ec70addf5c56724ce89df49ab17e27d63f
    len = read(fd, buf, sizeof(buf));
    //one boot sector must be 512B
    assert(len == BOOT_SIZE);
    //write boot sector to floppy img
<<<<<<< HEAD
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
    write_boot(argc[1]);
    write_setup(argc[2]);
    return 0;
=======
    len = write(STDOUT_FD, buf, SECTOR_SIZE);
    assert(len == SECTOR_SIZE);
	close(fd);
	return 0;
>>>>>>> 4776a2ec70addf5c56724ce89df49ab17e27d63f
}

