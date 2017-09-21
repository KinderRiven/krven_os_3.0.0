#include <types.h>

static uint32_t video_mem_start;
static uint32_t video_mem_end;
static uint32_t video_num_columns;
static uint32_t video_num_rows;


void con_write() {
	
}

void con_init() {
	video_mem_start = 0xB0000;
	video_mem_end = 0xB8000;
} 
