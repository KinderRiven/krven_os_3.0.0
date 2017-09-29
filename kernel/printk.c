#include <stdarg.h>
#include <kernel.h>

static char buf[1024];
extern int vsprintf(char *buf, const char *fmt, va_list args);

int printc(c_color_t bg, c_color_t font, const char *fmt, ...)
{
	va_list args;
	int i;
	
	va_start(args, fmt);		
	i = vsprintf(buf, fmt, args);
	con_color_write(bg, font, buf);
	va_end(args);
	
	return i;
}

int printk(const char *fmt, ...) 
{
	va_list args;
	int i;
	va_start(args, fmt);		
	i = vsprintf(buf, fmt, args);
	con_write(buf);
	va_end(args);
	return i;
}


