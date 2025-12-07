#include <stdio.h>
#include <stdlib.h>
#include "render.h"
#include "map.h"

// Clear screen cross-platform
void clearScreen() {
    #ifdef _WIN32
    system("cls");  // Windows: dùng cls
#else
    printf("\033[2J\033[1;1H");  // macOS/Linux: dùng ANSI codes
#endif
}

// Legacy functions (giữ để tương thích với code cũ)
void drawPlayer(int x, int y) {
    // Không dùng nữa - giờ dùng drawMap()
}

void drawEnemy(int x, int y) {
    // Không dùng nữa - giờ dùng drawMap()
}

// Vẽ map với player và enemy (1 enemy - chế độ cũ)
void drawMap(int playerX, int playerY, int enemyX, int enemyY, int enemyAlive) {
    printf("\n");
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        printf("  "); // Indent cho đẹp
        
        for (int x = 0; x < MAP_WIDTH; x++) {
            // Kiểm tra vị trí và vẽ
            if (x == playerX && y == playerY) {
                printf("\033[1;32m@\033[0m "); // Player màu xanh lá
            }
            else if (enemyAlive && x == enemyX && y == enemyY) {
                printf("\033[1;31mM\033[0m "); // Enemy (Monster) màu đỏ
            }
            else if (x == exitX && y == exitY) {
                printf("\033[1;33mE\033[0m "); // Exit màu vàng
            }
            else if (gameMap[y][x] == '#') {
                printf("\033[1;37m#\033[0m "); // Tường màu trắng
            }
            else {
                printf("  "); // Khoảng trống
            }
        }
        printf("\n");
    }
    
    printf("\n");
    printf("  \033[1;32m@\033[0m = Ban (Player)  ");
    printf("\033[1;31mM\033[0m = Quai vat (Monster)  ");
    printf("\033[1;33mE\033[0m = Loi thoat (Exit)\n");
}

// Vẽ map với player và NHIỀU enemies (chế độ mới)
void drawMapWithMultipleEnemies(int playerX, int playerY, Enemy *enemies, int numEnemies) {
    printf("\n");
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        printf("  "); // Indent cho đẹp
        
        for (int x = 0; x < MAP_WIDTH; x++) {
            // Kiểm tra player
            if (x == playerX && y == playerY) {
                printf("\033[1;32m@\033[0m ");
                continue;
            }
            
            // Kiểm tra TẤT CẢ enemies
            int isEnemy = 0;
            for (int i = 0; i < numEnemies; i++) {
                if (enemies[i].alive && x == enemies[i].x && y == enemies[i].y) {
                    printf("\033[1;31mM\033[0m ");
                    isEnemy = 1;
                    break;
                }
            }
            if (isEnemy) continue;
            
            // Kiểm tra exit
            if (x == exitX && y == exitY) {
                printf("\033[1;33mE\033[0m ");
            }
            // Tường
            else if (gameMap[y][x] == '#') {
                printf("\033[1;37m#\033[0m ");
            }
            // Khoảng trống
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
}