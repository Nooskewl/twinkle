#include <cstdio>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
static HANDLE console;
#else
#include <termios.h>
#endif
#include "twinkle.h"

namespace twinkle {

void start()
{
#ifdef _WIN32
	console = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void set(TWINKLE_COLOR fore, bool f_bright, TWINKLE_COLOR back, bool b_bright)
{
#ifdef _WIN32
	int c = (int)fore;
	if (f_bright) {
		c += 8;
	}
	int b = (int)back;
	b *= 16;
	c += b;
	if (b_bright) {
		c |= BACKGROUND_INTENSITY;
	}
	SetConsoleTextAttribute(console, c);
#else
	int c[8] = {
		0,
		4,
		2,
		6,
		1,
		5,
		3,
		7
	};

	if (f_bright) {
		printf("\x1b[3%d;1m", c[fore]);
	}
	else {
		printf("\x1b[3%dm", c[fore]);
	}

	if (b_bright) {
		printf("\x1b[48;5;%dm", c[back]+8);
	}
	else {
		printf("\x1b[4%dm", c[back]);
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
	return ::getch();
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

}
