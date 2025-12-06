#include <stdio.h>
#include <stdlib.h>
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
#include <time.h>
#include "enemy.h"
#include "map.h"

/* logic game */
#define MAX_OXY 100
#define TIME_LIMIT 180

/* draw oxy bar */
void drawOxyBar(int oxy) {
    int bars = 20;
    int filled = oxy * bars / 100;

    if (filled < 0) filled = 0;
    if (filled > bars) filled = bars;

    printf("OXY: [");
    for (int i = 0; i < bars; i++) {
        if (i < filled) printf("#");
        else printf("-");
    }
    printf("] %d%%\n", oxy);

    if (oxy > 0 && oxy <= 20)  {
        printf("\033[1;31mCANH BAO: SAP HET OXY!\n\033[0m");
    }
}


/* start game */
void startGame() {
    int running = 1;
    int x = playerStartX; 
    int y = playerStartY;
    /* call define oxy */
    int oxy = MAX_OXY;
    time_t startTime = time(NULL);
    srand((unsigned int)startTime);
    // Spawn with enemy từ map.c
    Enemy enemy;
    initEnemy(&enemy, enemyStartX, enemyStartY);
    while (running) {
        
        int elapsed = (int)(time(NULL) - startTime);
        #ifdef _WIN32
        system("cls"); // Windows
        #else
        system("clear"); // macOS/Linux
        #endif
        // OXY Giam theo thoi gian
        oxy = MAX_OXY - (elapsed * MAX_OXY / TIME_LIMIT);
        if (oxy < 0) oxy = 0;
        // Het OXY → Failed
        if (oxy <= 0) {
            oxy = 0;
            clearScreen();
            printf("\n=== HET OXY ===\n");
            printf("BAN DA HET OXY!\n");
            printf("GAME OVER!\n");
            printf("Thoi gian song: %d giay\n", elapsed);

            #ifdef _WIN32 // Windows
            Sleep(3000);

            #else
            usleep(3000000); // macOS/Linux

            #endif
            break;
        }
        
        // Cap nhat vi tri enemy
        updateEnemy(&enemy, x, y);
        // Kiem tra va cham
        checkEnemyCollision(&enemy, x, y, &running);
        if (!running) {
            clearScreen();
            printf("\n=== QUAI VAT ===\n");
            printf("BAN DA BI QUAI VAT BAT!\n");
            printf("GAME OVER!\n");
            printf("Thoi gian song: %d giay\n", elapsed);
            
            #ifdef _WIN32
            Sleep(3000);
            #else
            usleep(3000000);
            #endif
            break;
        }
        // Render map
        clearScreen();
        drawOxyBar(oxy);
        drawMap(x, y, enemy.x, enemy.y, enemy.alive);
        // xu ly input
        if (_kbhit()) {
            char key = _getch();
            handleInput(key, &x, &y, &running);
            if (isExit(x, y)) {
                clearScreen();
                printf("\n=== CHIEN THANG! ===\n");
                printf("Ban da thoat khoi me cung!\n");
                printf("Thoi gian: %d giay\n", elapsed);
                printf("OXY con lai: %d%%\n", oxy);
                #ifdef _WIN32 // Windows
                Sleep(3000);
                #else // macOS/Linux
                usleep(3000000);
                #endif
                running = 0;
                break;
            }
        }

        #ifdef _WIN32 // Windows
        Sleep(100);
        
        #else // macOS/Linux
        usleep(100000);

        #endif
    }
}
