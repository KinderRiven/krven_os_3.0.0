#include <types.h>
#include <string.h>
#include <io.h>

static uint32_t video_mem_start;
static uint32_t video_mem_end;
static uint32_t video_num_columns;	//lie
static uint32_t video_num_rows;		//hang

static uint16_t x, y;
static uint16_t pos;

static void move_cursor() {
	pos = video_num_columns * y + x;
	outb(0x3D4, 14);
	outb(0x3D5, pos >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, pos);
}

static void set_cursor(int new_x, int new_y) {
	pos = video_num_columns * new_y + new_x;
	outb(0x3D4, 14);
	outb(0x3D5, pos >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, pos);
}

static void write(char *buf) {
	int l = strlen(buf);		
}

void con_write() {
	
}

void con_init() {
	video_mem_start = 0xB0000;
	video_mem_end = 0xB8000;
	video_num_columns = 80;
	video_num_rows = 25;
	set_cursor(0, 0);
} 
