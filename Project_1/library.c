#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <termios.h>
#include <time.h>
#include "graphics.h"

#include <stdio.h>

int fd; //file descriptor of framebuffer
void* fb; //address that represents contents of file
struct fb_var_screeninfo fb_var; //pointer to struct
struct fb_fix_screeninfo fb_fix; //pointer to struct
struct termios termset;
int size;

void init_graphics(){
  fd = open("/dev/fb0", O_RDWR);
  if (fd < 0){
    write(1, "error opening\n", 20);
  }
  

  //get screen info
  ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
  ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);
  size = fb_var.yres_virtual * fb_fix.line_length;

	//
	fb = mmap(0, size, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
  if (fb == MAP_FAILED){
    write(1, "error mapping\n", 20);
  }
	
  //clear screen
  write(1, "\033[2J", 7);
	write(0, "\033[?25l", 7);

  //disable echo and ICANON
  ioctl(0, TCGETS, &termset);
  termset.c_lflag = 2101;
  ioctl(0, TCSETS, &termset);

}

void exit_graphics(){
	//re-enable keypress and ICANON
  termset.c_lflag = 2111;
  ioctl(0, TCSETS, &termset);
	
	//bring back cursor
	write(1, "\033[?25h", 7);
}

char getkey(){
  fd_set reader;
  struct timeval tv;
  int val;
  void* buf;
  //make select watch for input
  FD_ZERO(&reader);
  FD_SET(0, &reader);

  //stop select from blocking
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  //call select
  val = select(1, &reader, NULL, NULL, &tv);

  if (val == 1){
    read(1, &buf, 1);
  }

  return (char)((int)buf);
}

void sleep_ms(long ms){
	struct timespec tv;
	int sec = 0;
	sec = ms / 1000;
	//parameter is in ms, need to convert to ns	
	tv.tv_sec = sec;
	tv.tv_nsec = (ms * 1000000);
	nanosleep(&tv, NULL);
}

//call this in snake.c?
void* new_offscreen_buffer(){
	void* img;
	img = mmap(0, size, PROT_WRITE|PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	return img;
}


//src is the new_offscreen_buffer, copy all bytes into fb
void blit(void* src){
	int i = 0;
	for (i; i < size / 2 - 1; i++){
		((color_t *)fb)[i] = ((color_t *)src)[i];
	}
}

//img is offscreen buffer, need to convert to array of color_t 
void draw_pixel(void* img, int x, int y, color_t color){
	if (x > 639 || y > 479 || x < 0 || y < 0){
		//do nothing, coordinates are invalid
	}else{
		((color_t *)img)[x + (640 * y)] = color;
	}
}

//bresenham's algorithm, modified by KLG92. source is wikipedia
void draw_line(void* img, int x1, int y1, int x2, int y2, color_t c){
	int dx, dy, p, x, y;
	//put the greater x coordinate first
	if (x1 > x2){
		int tempx = x1;
		int tempy = y1;
		x1 = x2;
		y1 = y2;
		x2 = tempx;
		y2 = tempy;
	}
  dx = x2-x1;
  dy = y2-y1;
 
  x=x1;
  y=y1;
 
  p = 2*dy-dx;
	if (x1 == x2){
		//put the greater y coordinate first
		if (y1 > y2){
			int temp = y1;
			y1 = y2;
			y2 = temp;
			y = y1;
		}
		while(y < y2){
			draw_pixel(img, x, y, c);
			y = y+1;
		}
	}
  while(x<x2){
    if(p>=0){
      draw_pixel(img, x, y, c);
      y=y+1;
      p=p+2*dy-2*dx;
    }
    else{
      draw_pixel(img, x, y, c);
      p=p+2*dy;
    }
    x=x+1;
  }
}

//make everything in img 0
void clear_screen(void* img){
	int i = 0;
	for (i; i < size / 2 - 1; i++){
		((color_t *)img)[i] = RGB(0, 0, 0);
	}
}

//testing purposes

/* int main(){
  init_graphics();
	void* buff = new_offscreen_buffer();
	draw_line(buff, 100, 100, 300, 300, RGB(0, 63, 10));
	blit(buff);
  while (1 == 1){
    char c = getkey();
		printf("%d\n", c);
    if (c == 32){
      break;
    }
  }
	clear_screen(fb);
  exit_graphics();
  return 0;
} */
