#include <stdio.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

/* POSIX implementation of _kbhit() */
static int kbhit_posix(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return 0;
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();
    if (ch != EOF) {
        ungetc(ch, stdin);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        return 1;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return 0;
}

/* POSIX implementation of _getch() */
static int getch_posix(void) {
    struct termios oldt, newt;
    int ch;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return EOF;
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

/* map the names used in Windows code to POSIX versions */
#define _kbhit() kbhit_posix()
#define _getch() getch_posix()

/* map Sleep(ms) to usleep on POSIX */
#define Sleep(ms) usleep((ms) * 1000)
#endif
#include "game.h"
#include "render.h"
#include "input.h"

void startGame() {
    int running = 1;
    int x = 5, y = 5;

    while (running) {
        clearScreen();
        drawPlayer(x, y);

        if (_kbhit()) {
            char key = _getch();
            handleInput(key, &x, &y, &running);
        }

        Sleep(50);
    }
}
