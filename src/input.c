#include "input.h"
#include "map.h"
#include <stdio.h>

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Giả lập _kbhit() cho macOS/Linux
int _kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// Giả lập _getch() cho macOS/Linux
int _getch(void) {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

void handleInput(char key, int *x, int *y, int *running) {
    int newX = *x;
    int newY = *y;

    switch (key) {
        case 'w':
        case 'W': newY--; break;
        case 's':
        case 'S': newY++; break;
        case 'a':
        case 'A': newX--; break;
        case 'd':
        case 'D': newX++; break;
        case 'q':
        case 'Q':
            *running = 0;
            return;
    }
    // Kiểm tra va chạm tường
    if (!isWall(newX, newY) || isExit(newX, newY)) {
        *x = newX;
        *y = newY;
    }


    // Kiểm tra thoát mê cung
    if (isExit(*x, *y)) {
        *running = 0;
        printf("\n=== YOU ESCAPED! ===\n");
        printf("BAN DA THOAT KHOI ME CUNG!\n");
    }
}
