#include "enemy.h"
#include "map.h"
#include <stdio.h>


static int moveCounter = 0;
#define ENEMY_SPEED 1

void initEnemy(Enemy *e, int x, int y) {
    e->x = x;
    e->y = y;
    e->alive = 1;
}

void updateEnemy(Enemy *e, int playerX, int playerY) {
    if (!e->alive) return;

    moveCounter++;
    if (moveCounter < ENEMY_SPEED) return;
    moveCounter = 0;

    int newX = e->x;
    int newY = e->y;

    // Quái đuổi theo X
    if (e->x < playerX) newX++;
    else if (e->x > playerX) newX--;

    // Quái đuổi theo Y
    if (e->y < playerY) newY++;
    else if (e->y > playerY) newY--;

    if (!isWall(newX, newY)) {
        e->x = newX;
        e->y = newY;
    }
}


void checkEnemyCollision(Enemy *e, int playerX, int playerY, int *running) {
    if (e->alive && e->x == playerX && e->y == playerY) {
        // printf("\n=== BI QUAI TAN CONG ===\n");
        // printf("BAN DA BI HA!\n");
        e->alive = 0;
        *running = 0;
    }
}
