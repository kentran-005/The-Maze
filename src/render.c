#include <stdio.h>
#include <stdlib.h>
#include "render.h"
#include "map.h"

#ifdef _WIN32
#include <windows.h>
#endif

void clearScreen() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);

    SetConsoleCursorPosition(hConsole, homeCoords);
#else
    printf("\033[2J\033[H");
    fflush(stdout);
#endif
}

void drawPlayer(int x, int y) {
    // Legacy - không dùng
}

void drawEnemy(int x, int y) {
    // Legacy - không dùng
}

// Vẽ map với 1 enemy (legacy)
void drawMap(int playerX, int playerY, int enemyX, int enemyY, int enemyAlive) {
    printf("\n");
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        printf("  ");
        
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == playerX && y == playerY) {
                printf("\033[1;36m😎\033[0m");
            }
            else if (enemyAlive && x == enemyX && y == enemyY) {
                printf("\033[1;31m👻\033[0m");
            }
            else if (x == exitX && y == exitY) {
                printf("\033[1;33m🏆\033[0m");
            }
            else if (gameMap[y][x] == '#') {
                printf("\033[1;37m##\033[0m");
            }
            else {
                printf("  ");
            }
        }
        printf("\n");
    }
    
    printf("\n");
    printf("  \033[1;36m😎\033[0m = Ban   ");
    printf("\033[1;31m👻\033[0m = Quai   ");
    printf("\033[1;33m🏆\033[0m = Exit\n");
}

// Vẽ map với nhiều enemies
void drawMapWithMultipleEnemies(int playerX, int playerY, Enemy *enemies, int numEnemies, int playerFacing) {
    printf("\n");
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        printf("  ");  // Indent
        
        for (int x = 0; x < MAP_WIDTH; x++) {
            // === VẼ PLAYER ===
            if (x == playerX && y == playerY) {
                // playerFacing: 0 = trái (<), 1 = phải (>)
                const char* icon;
                switch (playerFacing)
                {
                case 0: icon = "^"; break; // Lên
                case 1: icon = ">"; break; // Phải
                case 2: icon = "v"; break; // Xuống
                case 3: icon = "<"; break; // Trái
                
                default: icon = "@";
                }
                printf("\033[1;36m%s\033[0m ", icon);
                continue;
            }

            // === VẼ ENEMIES ===
            int isEnemy = 0;
            for (int i = 0; i < numEnemies; i++) {
                if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y) {
                    // Enemy facing: 0=lên, 1=phải, 2=xuống, 3=trái
                    const char* icon = "M";
                    switch(enemies[i].facing) {
                        case 0: icon = "^"; break;  // Lên
                        case 1: icon = ">"; break;  // Phải
                        case 2: icon = "v"; break;  // Xuống
                        case 3: icon = "<"; break;  // Trái
                        default: icon = "M";
                    }
                    printf("\033[1;31m%s\033[0m ", icon);
                    isEnemy = 1;
                    break;
                }
            }
            if (isEnemy) continue;

            // === VẼ EXIT ===
            if (x == exitX && y == exitY) {
                printf("\033[1;33mE\033[0m ");
            }
            // === VẼ TƯỜNG ===
            else if (gameMap[y][x] == '#') {
                printf("\033[1;90m█\033[0m ");  // Màu xám đậm
            }
            // === KHOẢNG TRỐNG ===
            else {
                printf("  ");
            }
        }
        printf("\n");
    }

    printf("\n");
    printf("  \033[1;36m< >\033[0m = Ban (Player)   ");
    printf("\033[1;31m^ > v <\033[0m = Quai vat   ");
    printf("\033[1;33mE\033[0m = Exit\n");

    fflush(stdout);
}