#define RGB(r, g, b) ((color_t) (r << 11) + (g << 5) + b)

typedef unsigned short color_t;
void init_graphics();
void exit_graphics();
char getkey();
void sleep_ms(long ms);
void* new_offscreen_buffer();
void blit(void* src);
void draw_pixel(void* img, int x, int y, color_t color);
void draw_line(void* img, int x1, int y1, int x2, int y2, color_t c);
void clear_screen(void* img);