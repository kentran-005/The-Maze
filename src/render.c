#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "render.h"
#include "map.h"

#ifdef _WIN32
#include <windows.h>
#endif

// Di chuyển con trỏ về (0,0); tránh xóa toàn bộ buffer (đỡ giật)
void clearScreen() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD home = {0, 0};
    SetConsoleCursorPosition(hOut, home);
    DWORD dwSize = 0;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    dwSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD coord = {0, 0};
    DWORD cCharsWritten;
    FillConsoleOutputCharacter(hOut, ' ', dwSize, coord, &cCharsWritten);
    SetConsoleCursorPosition(hOut, home);
#else
    printf("\033[2J\033[H");
    fflush(stdout);
#endif
}

// Append an entire formatted chunk into buffer (truncates if full)
static void appendFmt(char *buf, size_t bufSize, size_t *offset, const char *fmt, ...) {
    if (*offset >= bufSize) return;
    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(buf + *offset, bufSize - *offset, fmt, args);
    va_end(args);
    if (written < 0) return;
    if ((size_t)written > bufSize - *offset) {
        *offset = bufSize; // mark as full
    } else {
        *offset += (size_t)written;
    }
}

static const char *staticCells[MAP_HEIGHT][MAP_WIDTH];
static int staticInited = 0;

static void ensureStaticCells() {
    if (staticInited) return;

    static const char *wallTok  = "\033[1;90m█\033[0m ";
    static const char *exitTok  = "\033[1;33mE\033[0m ";
    static const char *spaceTok = "  ";
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == exitX && y == exitY) staticCells[y][x] = exitTok;
            else if (gameMap[y][x] == '#') staticCells[y][x] = wallTok;
            else staticCells[y][x] = spaceTok;
        }
    }
    staticInited = 1;
}

// Vẽ map với 1 enemy (legacy)
// void drawMap(int playerX, int playerY, int enemyX, int enemyY, int enemyAlive) {
//     printf("\n");
    
//     for (int y = 0; y < MAP_HEIGHT; y++) {
//         printf("  ");
        
//         for (int x = 0; x < MAP_WIDTH; x++) {
//             if (x == playerX && y == playerY) {
//                 printf("\033[1;36m😎\033[0m");
//             }
//             else if (enemyAlive && x == enemyX && y == enemyY) {
//                 printf("\033[1;31m👻\033[0m");
//             }
//             else if (x == exitX && y == exitY) {
//                 printf("\033[1;33m🏆\033[0m");
//             }
//             else if (gameMap[y][x] == '#') {
//                 printf("\033[1;37m##\033[0m");
//             }
//             else {
//                 printf("  ");
//             }
//         }
//         printf("\n");
//     }
    
//     printf("\n");
//     printf("  \033[1;36m😎\033[0m = Ban   ");
//     printf("\033[1;31m👻\033[0m = Quai   ");
//     printf("\033[1;33m🏆\033[0m = Exit\n");
// }

// Vẽ map với nhiều enemies
// ===== NỀN TĨNH (tường/exit/khoảng trống) =====

void drawMapWithMultipleEnemies(int playerX, int playerY, Enemy *enemies, int numEnemies, int playerFacing) {
    ensureStaticCells();

    // Buffer hóa để giảm số lần write ra terminal
    char buffer[20000];
    size_t off = 0;

    appendFmt(buffer, sizeof(buffer), &off, "\n");
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        appendFmt(buffer, sizeof(buffer), &off, "  ");
        
        for (int x = 0; x < MAP_WIDTH; x++) {
            // === VẼ PLAYER ===
            if (x == playerX && y == playerY) {
                const char* icon = "@";
                switch (playerFacing)
                {
                case 0: icon = "^"; break; // Lên
                case 1: icon = ">"; break; // Phải
                case 2: icon = "v"; break; // Xuống
                case 3: icon = "<"; break; // Trái
                }
                appendFmt(buffer, sizeof(buffer), &off, "\033[1;36m%s\033[0m ", icon);
                continue;
            }

            // === VẼ ENEMIES ===
            int isEnemy = 0;
            for (int i = 0; i < numEnemies; i++) {
                if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y) {
                    const char* icon = "M";
                    switch(enemies[i].facing) {
                        case 0: icon = "^"; break;
                        case 1: icon = ">"; break;
                        case 2: icon = "v"; break;
                        case 3: icon = "<"; break;
                    }
                    appendFmt(buffer, sizeof(buffer), &off, "\033[1;31m%s\033[0m ", icon);
                    isEnemy = 1;
                    break;
                }
            }
            if (isEnemy) continue;

            // Nền tĩnh đã chuẩn bị
            appendFmt(buffer, sizeof(buffer), &off, "%s", staticCells[y][x]);
        }
        appendFmt(buffer, sizeof(buffer), &off, "\n");
    }

    
    appendFmt(buffer, sizeof(buffer), &off, "\n^ > v < = Ban | ^ > v < = Quai | E = Exit\n");

    fwrite(buffer, 1, off, stdout);
    fflush(stdout);
}
