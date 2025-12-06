#include <stdio.h>
#include <stdlib.h>
#include "render.h"
#include "map.h"

// Clear screen cross-platform
void clearScreen() {
    printf("\033[2J\033[1;1H");
}

// Legacy functions (giữ để tương thích với code cũ)
void drawPlayer(int x, int y) {
    // Không dùng nữa - giờ dùng drawMap()
}

void drawEnemy(int x, int y) {
    // Không dùng nữa - giờ dùng drawMap()
}

// Vẽ map với player và enemy
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