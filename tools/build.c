/* * * * * * * * * * * * *
 * Create by kinderriven *
 * 2017/9/13             *
 * * * * * * * * * * * * */

#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define STDIN_FD        0
#define STDOUT_FD       1
#define SECTOR_SIZE     512
#define BOOT_SIZE       (SECTOR_SIZE * 1)
#define SETUP_SIZE      (SECTOR_SIZE * 4)

//write boot
void write_boot(char *boot) {
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

//write setup
void write_setup(char *setup) {
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

//write kernel
void write_kernel(char *kernel) {
    int fd, read_len, write_len;
    char buf[SECTOR_SIZE];
	
	assert((fd = open(kernel, O_RDONLY)) > 0);
	
	while((read_len = read(fd, buf, sizeof(buf))) > 0) {
		write_len = write(STDOUT_FD, buf, read_len);
		assert(read_len == write_len);
	}
    
	close(fd);
}

int main(int argc, char **argv) {
    //check
    assert(argc >= 4);
    write_boot(argv[1]);
    write_setup(argv[2]);
    write_kernel(argv[3]);
	return 0;
}

