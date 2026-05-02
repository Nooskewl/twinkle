#include <cstdio>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
static HANDLE console;
#else
#include <termios.h>
#endif
#include "twinkle.h"

#ifdef _WIN32
int win_fore[8] = {
	0,
	FOREGROUND_BLUE,
	FOREGROUND_GREEN,
	FOREGROUND_BLUE | FOREGROUND_GREEN,
	FOREGROUND_RED,
	FOREGROUND_BLUE | FOREGROUND_RED,
	FOREGROUND_RED | FOREGROUND_GREEN,
	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN
};
int win_back[8] = {
	0,
	BACKGROUND_BLUE,
	BACKGROUND_GREEN,
	BACKGROUND_BLUE | BACKGROUND_GREEN,
	BACKGROUND_RED,
	BACKGROUND_BLUE | BACKGROUND_RED,
	BACKGROUND_RED | BACKGROUND_GREEN,
	BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN
};
#else
int linux_colours[8] = {
	0,
	4,
	2,
	6,
	1,
	5,
	3,
	7
};
#endif

namespace twinkle {

void start()
{
#ifdef _WIN32
	console = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void set_fore(TWINKLE_COLOR colour, bool bright)
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(console, &bi);

	int c = win_fore[colour];
	if (bright) {
		c |= FOREGROUND_INTENSITY;
	}

	c |= bi.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);

	SetConsoleTextAttribute(console, c);
#else
	if (bright) {
		printf("\x1b[3%d;1m", linux_colours[colour]);
	}
	else {
		printf("\x1b[3%dm", linux_colours[colour]);
	}
#endif
}

void set_back(TWINKLE_COLOR colour, bool bright)
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(console, &bi);

	int c = win_back[colour];
	if (bright) {
		c |= BACKGROUND_INTENSITY;
	}

	c |= bi.wAttributes & (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	SetConsoleTextAttribute(console, c);
#else
	if (bright) {
		printf("\x1b[48;5;%dm", linux_colours[colour]+8);
	}
	else {
		printf("\x1b[4%dm", linux_colours[colour]);
	}
#endif
}

void reset()
{
#ifdef _WIN32
	SetConsoleTextAttribute(console, WHITE);
#else
	printf("\x1b[0m");
#endif
}

void clear()
{
#ifdef _WIN32
	COORD topleft = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topleft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topleft, &written
	);
	SetConsoleCursorPosition(console, topleft);
#else
	printf("\e[1;1H\e[2J");
#endif
}

int getch()
{
#ifdef _WIN32
	return _getch();
#else
	struct termios old, current;
	tcgetattr(0, &old);
	current = old;
	current.c_lflag &= ~ICANON;
	current.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &current);
	int c = getchar();
	tcsetattr(0, TCSANOW, &old);
	return c;
#endif
}

int kbhit()
{
	return _kbhit();
}

}
