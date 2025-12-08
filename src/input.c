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

void handleInput(char key, int *x, int *y, int *running, int *playerFacing) {
    int newX = *x;
    int newY = *y;

    switch (key) {
        case 'w': 
        case 'W': 
            newY--; 
            // playerFacing CHỈ CÓ 2 GIÁ TRỊ: 0=trái, 1=phải
            // Không thay đổi khi di chuyển lên/xuống
            break;
            
        case 's': 
        case 'S': 
            newY++; 
            // Không thay đổi facing
            break;
            
        case 'a': 
        case 'A': 
            newX--; 
            *playerFacing = 0;  // Quay trái
            break;
            
        case 'd': 
        case 'D': 
            newX++; 
            *playerFacing = 1;  // Quay phải
            break;
            
        case 'q': 
        case 'Q': 
            *running = 0; 
            return;  // Thoát ngay
            
        default: 
            return;  // Phím không hợp lệ
    }
    
    // Kiểm tra tường (cho phép đi vào exit)
    if (!isWall(newX, newY)) {
        *x = newX;
        *y = newY;
    }
    
    // Không check exit ở đây, để game.c lo
}