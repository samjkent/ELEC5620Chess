#include "vga_drv.h"

// TODO: Add dual buffer support to remove flicker

int x,y,offset;
volatile short * vga_pixel_buffer = (short *) 0xC8000000;
short colour_test = 0xF0;

void vga_write_pixel(int x, int y, short colour, int x_inv){

	if(x_inv) y = 240 - y;

	offset = (y << 9) + x;
	*(vga_pixel_buffer + offset) = colour; // compute halfword address, set pixel

}

void vga_draw_test(void){
	for (x = 0; x < 320; x++){
		for(y = 0; y < 240; y++){
			colour_test = ~colour_test;
			vga_write_pixel(x,y,colour_test, 1);
		}
	}
}

void vga_clear_screen(void){
	for (x = 0; x < 320; x++){
		for(y = 0; y < 240; y++){
			vga_write_pixel(x,y,0, 0);
		}
	}
}

