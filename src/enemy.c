#include "enemy.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ENEMY_SPEED 2
#define CHASE_RANGE 8
#define DIR_CHANGE 8
#define RANDOM_CHANCE 30

void initEnemy(Enemy *e, int x, int y) {
    e->x = x;
    e->y = y;
    e->alive = 1;
    
    // Khởi tạo các biến RIÊNG của enemy này
    e->moveCounter = 0;
    e->currentDir = rand() % 4;
    e->dirCounter = 0;
    e->historyIndex = 0;
    
    for (int i = 0; i < HISTORY_SIZE; i++) {
        e->historyX[i] = -1;
        e->historyY[i] = -1;
    }
}

int getDistance(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
}

int isInHistory(Enemy *e, int x, int y) {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (e->historyX[i] == x && e->historyY[i] == y) {
            return 1;
        }
    }
    return 0;
}

void addToHistory(Enemy *e, int x, int y) {
    e->historyX[e->historyIndex] = x;
    e->historyY[e->historyIndex] = y;
    e->historyIndex = (e->historyIndex + 1) % HISTORY_SIZE;
}

void updateEnemy(Enemy *e, int playerX, int playerY) {
    if (!e->alive) return;

    e->moveCounter++;
    if (e->moveCounter < ENEMY_SPEED) return;
    e->moveCounter = 0;

    int dist = getDistance(e->x, e->y, playerX, playerY);
    
    int directions[4][2] = {
        {0, -1},  // 0: Lên
        {1, 0},   // 1: Phải
        {0, 1},   // 2: Xuống
        {-1, 0}   // 3: Trái
    };
    
    // MODE 1: CHASE - Player ở gần
    if (dist <= CHASE_RANGE) {
        int useRandom = (rand() % 100) < RANDOM_CHANCE;
        
        if (useRandom) {
            // DI CHUYỂN RANDOM
            int validDirs[4];
            int validCount = 0;
            
            for (int i = 0; i < 4; i++) {
                int newX = e->x + directions[i][0];
                int newY = e->y + directions[i][1];
                
                if (!isWall(newX, newY) && !isInHistory(e, newX, newY)) {
                    validDirs[validCount++] = i;
                }
            }
            
            if (validCount > 0) {
                int randomDir = validDirs[rand() % validCount];
                addToHistory(e, e->x, e->y);
                e->x += directions[randomDir][0];
                e->y += directions[randomDir][1];
                e->currentDir = randomDir;
                e->dirCounter = 0;
            }
        } else {
            // ĐUỔI PLAYER
            int bestDir = -1;
            int bestDist = 999;
            
            for (int i = 0; i < 4; i++) {
                int newX = e->x + directions[i][0];
                int newY = e->y + directions[i][1];
                
                if (isWall(newX, newY)) continue;
                
                int newDist = getDistance(newX, newY, playerX, playerY);
                if (newDist < bestDist) {
                    bestDist = newDist;
                    bestDir = i;
                }
            }
            
            if (bestDir != -1) {
                addToHistory(e, e->x, e->y);
                e->x += directions[bestDir][0];
                e->y += directions[bestDir][1];
                e->currentDir = bestDir;
                e->dirCounter = 0;
            }
        }
    }
    // MODE 2: PATROL - Player ở xa
    else {
        e->dirCounter++;
        
        int newX = e->x + directions[e->currentDir][0];
        int newY = e->y + directions[e->currentDir][1];
        int willBeStuck = isInHistory(e, newX, newY);
        
        int suddenChange = (rand() % 100) < (RANDOM_CHANCE / 2);
        
        if (isWall(newX, newY) || e->dirCounter >= DIR_CHANGE || willBeStuck || suddenChange) {
            int validDirs[4];
            int validCount = 0;
            
            for (int i = 0; i < 4; i++) {
                int testX = e->x + directions[i][0];
                int testY = e->y + directions[i][1];
                
                if (!isWall(testX, testY) && !isInHistory(e, testX, testY)) {
                    validDirs[validCount++] = i;
                }
            }
            
            if (validCount > 0) {
                int randomDir = validDirs[rand() % validCount];
                e->currentDir = randomDir;
                addToHistory(e, e->x, e->y);
                e->x += directions[randomDir][0];
                e->y += directions[randomDir][1];
                e->dirCounter = 0;
            }
        }
        else {
            addToHistory(e, e->x, e->y);
            e->x = newX;
            e->y = newY;
        }
    }
}

void checkEnemyCollision(Enemy *e, int playerX, int playerY, int *running) {
    if (e->alive && e->x == playerX && e->y == playerY) {
        e->alive = 0;
        *running = 0;
    }
}