#include <types.h>
#include <string.h>
#include <io.h>
#include <console.h>

static uint32_t video_mem_start;
static uint32_t video_mem_end;
static uint32_t video_num_columns;	//lie
static uint32_t video_num_rows;		//hang

static uint16_t *video_mem_ptr;

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
	x = new_x;
	y = new_y;
}

static void write_color_ch(c_color_t bg, c_color_t font, char ch) {
	uint16_t color = ((uint16_t)(bg << 4) | (font & 0xF)) << 8;
	if(ch == '\n') {
		x = 0;
		y++;
	}
	else {
		pos = video_num_columns * y + x;
		video_mem_ptr[pos] = color | ch;
		x++;
		if(x == video_num_columns) {
			x = 0;
			y++;
		}	
	}
	move_cursor();
}

static void write_ch(char ch) {
	write_color_ch(c_black, c_white, ch);			
}

static void write_str(char *str) {
	while(*str) {
		write_ch(*str++);
	}
}

static void write_color_str(c_color_t bg, c_color_t font, char *str) {
	while(*str) {
		write_color_ch(c_black, c_white, *str++);
	}
}

static void con_clear() {
	int size = video_num_columns * video_num_rows;
	set_cursor(0, 0);
	while(size--) {
		write_ch(' ');
	}
	set_cursor(0, 0);	
}

void con_write() {
		
}

void con_init() {
	video_mem_start = 0xB8000;
	video_num_columns = 80;
	video_num_rows = 25;
	video_mem_ptr = (uint16_t *)video_mem_start;
	con_clear();
} 
