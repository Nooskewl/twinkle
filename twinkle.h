#ifndef TWINKLE_H
#define TWINKLE_H

#ifdef TWINKLE_BRITISH
#define TWINKLE_COLOUR TWINKLE_COLOR
#endif

namespace twinkle {

enum TWINKLE_COLOR {
	BLACK = 0,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PURPLE,
	YELLOW,
	WHITE
};

void start();
void set_fore(TWINKLE_COLOR c, bool bright);
void set_back(TWINKLE_COLOR c, bool bright);
void reset();
void clear();

int getch();
int kbhit();

void set_cursor_pos(int x, int y);

void get_console_size(int *w, int *h);

}

#endif // TWINKLE_H
