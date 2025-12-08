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

void drawMapWithMultipleEnemies(int playerX, int playerY, Enemy *enemies, int numEnemies, int playerFacing)
{
    const char* playerIcons[4] = {"\033[1;32m^\033[0m", "\033[1;32m>\033[0m", "\033[1;32mv\033[0m", "\033[1;32m<\033[0m"};
    const char* enemyIcons[4]  = {"\033[1;31mM^\033[0m", "\033[1;31mM>\033[0m", "\033[1;31mMv\033[0m", "\033[1;31mM<\033[0m"};

    printf("\n");
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            // === VẼ PLAYER THEO HƯỚNG ===
            if (x == playerX && y == playerY) {
                printf("\033[1;32m%s\033[0m ", playerIcons[playerFacing]);
                continue;
            }

            // === VẼ ENEMY THEO HƯỚNG ===
            int isEnemy = 0;
            for (int i = 0; i < numEnemies; i++) {
                if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y) {
                    printf("\033[1;31m%s\033[0m ", enemyIcons[enemies[i].facing]);
                    isEnemy = 1;
                    break;
                }
            }
            if (isEnemy) continue;

            // === VẼ EXIT & TƯỜNG ===
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
    printf(" \033[1;32m^ > v <\033[0m = Ban (Player)    ");
    printf("\033[1;31mM^ M> Mv M<\033[0m = Quai vat    ");
    printf("\033[1;33mE\033[0m = Loi thoat\n");
    printf(" Mat doi mat moi giet duoc quai!\n");

    fflush(stdout);
}
