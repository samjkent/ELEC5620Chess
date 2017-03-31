#include "vga_drv.h"

void vga_write_pixel(int x, int y, short colour){
	volatile short *vga_pixel = (volatile short*)(VGA_BASE_ADDR + (y<<10) + (x << 1));
	*vga_pixel = colour;
}
