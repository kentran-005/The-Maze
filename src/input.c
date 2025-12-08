#include "input.h"
#include "map.h"
#include <stdio.h>

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "render.h"

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

void handleInput(char key, int *x, int *y, int *running, int *playerFacing) {
    int newX = *x;
    int newY = *y;
    int moved = 0;

    switch (key) {
        case 'w': case 'W': newY--; *playerFacing = 0; break;  // lên
        case 's': case 'S': newY++; *playerFacing = 2; break;  // xuống
        case 'a': case 'A': newX--; *playerFacing = 3; break;  // trái
        case 'd': case 'D': newX++; *playerFacing = 1; break;  // phải
        case 'q': case 'Q': *running = 0; break;
        return;
        default: return;
    }
    
    // Kiểm tra va chạm tường
    if (!isWall(newX, newY) || isExit(newX, newY)) {
        *x = newX;
        *y = newY;
        moved = 1;
    }


    // Kiểm tra thoát mê cung
    if (isExit(*x, *y)) {
        *running = 0;
        clearScreen();
        printf("\n=== YOU ESCAPED! ===\n");
        printf("BAN DA THOAT KHOI ME CUNG!\n");

#ifdef _WIN32
        Sleep(5000);
#else
        usleep(5000000);
#endif
    }
}
