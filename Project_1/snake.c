#include "graphics.h"
#include <stdio.h>

//get keypress, then move accordingly, then blit then wait?
int main(){
	//coordinates of player
	int x = 320;
	int y = 240;
	color_t GREEN = RGB(0, 63, 0);
	color_t BLACK = RGB(0, 0, 0);
	printf("\nIMPORTANT: controls are wasd\npress space to exit game\n");
	printf("do you wish to start? [y/n]\n");
	while(1 == 1){
		char c = getkey();
		if (c == 121){
			break;
		}
	}
	init_graphics();
	void* img = new_offscreen_buffer();
	draw_pixel(img, x, y, GREEN);
	while (1 == 1){
		char c = getkey();
		if (c == 32){
			break;
		}else if (c == 119){ //key is w
			draw_pixel(img, x, y, BLACK);
			if (y == 0){
				y = 479;
			}else{
				y--;
			}
			draw_pixel(img, x, y, GREEN);
		}else if (c == 97){ //key is a 
			draw_pixel(img, x, y, BLACK);
			if(x == 0){
				x = 639;
			}else{
				x--;
			}
			draw_pixel(img, x, y, GREEN);
		}else if (c == 115){ //key is s
			draw_pixel(img, x, y, BLACK);
			if (y == 479){
				y = 0;
			}else{
				y++;
			}
			draw_pixel(img, x, y, GREEN);
		}else if (c == 100){ // key is d
		  draw_pixel(img, x, y, BLACK);
			if (x == 639){
				x = 0;
			}else{
				x++;
			}
			draw_pixel(img, x, y, GREEN);
		}
		blit(img);
		sleep_ms(10);
	}
	exit_graphics();
}