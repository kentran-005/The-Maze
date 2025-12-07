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

void drawPlayer(int x, int y) {}
void drawEnemy(int x, int y) {}

// Vẽ map với player và enemy (1 enemy - chế độ cũ)
void drawMap(int playerX, int playerY, int enemyX, int enemyY, int enemyAlive) {
    printf("\n");
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        printf("  ");
        
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == playerX && y == playerY) {
                printf("\033[1;32m@\033[0m ");
            }
            else if (enemyAlive && x == enemyX && y == enemyY) {
                printf("\033[1;31mM\033[0m ");
            }
            else if (x == exitX && y == exitY) {
                printf("\033[1;33mE\033[0m ");
            }
            else if (gameMap[y][x] == '#') {
                printf("\033[1;37m#\033[0m ");
            }
            else {
                printf("  ");
            }
        }
        printf("\n");
    }
    
    printf("\n");
    printf("  \033[1;32m@\033[0m = Ban (Player)  ");
    printf("\033[1;31mM\033[0m = Quai vat (Monster)  ");
    printf("\033[1;33mE\033[0m = Loi thoat (Exit)\n");
}

void drawMapWithMultipleEnemies(int playerX, int playerY, Enemy *enemies, int numEnemies) {
    printf("\n");
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        printf("  ");
        
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == playerX && y == playerY) {
                printf("\033[1;32m@\033[0m ");
                continue;
            }
            
            int isEnemy = 0;
            for (int i = 0; i < numEnemies; i++) {
                if (enemies[i].alive && x == enemies[i].x && y == enemies[i].y) {
                    printf("\033[1;31mM\033[0m ");
                    isEnemy = 1;
                    break;
                }
            }
            if (isEnemy) continue;
            
            if (x == exitX && y == exitY) {
                printf("\033[1;33mE\033[0m ");
            }
            else if (gameMap[y][x] == '#') {
                printf("\033[1;37m#\033[0m ");
            }
            else {
                printf("  ");
            }
        }
        printf("\n");
    }
    
    printf("\n");
    printf("  \033[1;32m@\033[0m = Ban (Player)   ");
    printf("\033[1;31mM\033[0m = Quai vat (Monster)   ");
    printf("\033[1;33mE\033[0m = Loi thoat (Exit)\n");
    
    fflush(stdout); // Force output flush to prevent buffering issues
}
