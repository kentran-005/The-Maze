#include "enemy.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ENEMY_SPEED 2
#define CHASE_RANGE 8
#define DIR_CHANGE 8
#define RANDOM_CHANCE 30
#define RESPAWN_TIME 50 // 50 ticks = 5 seconds

void initEnemy(Enemy *e, int x, int y) {
    e->x = x;
    e->y = y;
    e->alive = 1;
    
    e->moveCounter = 0;
    e->currentDir = rand() % 4;
    e->dirCounter = 0;
    e->historyIndex = 0;
    
    e->facing = e->currentDir;  // facing = currentDir (0-3)

    e->spawnX = x;
    e->spawnY = y;
    e->respawnTimer = 0;

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

int isValidMove(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return 0;
    }
    return !isWall(x, y);
}

void updateEnemyRespawn(Enemy *e, int playerX, int playerY) {
    if (e->alive) return;
    
    e->respawnTimer++;
    if (e->respawnTimer >= RESPAWN_TIME)
    // Tim vi tri spawn khong gan player
    {
        int spawnX, spawnY;
        int found = 0;
        int attempts = 0;

        while (!found && attempts < 20)
        {
            spawnX = 2 + rand() % (MAP_WIDTH - 10);
            spawnY = 2 + rand() % (MAP_HEIGHT - 10);

            // check tuong va khoang cach den player (>=5)
            if (!isWall(spawnX, spawnY) && getDistance(spawnX, spawnY, playerX, playerY) >= 5)
            {
                found = 1;
            }
            attempts++;
        }
        if (found) {
            initEnemy(e, spawnX, spawnY);
            printf("\033[1;33m[+] Quai vat da hoi sinh lai!\033[0m\n");
        }
    }
}

void updateEnemy(Enemy *e, int playerX, int playerY) {
    if (!e->alive) return;

    e->moveCounter++;
    if (e->moveCounter < ENEMY_SPEED) return;
    e->moveCounter = 0;

    int dist = getDistance(e->x, e->y, playerX, playerY);
    
    // 4 hướng: 0=Lên, 1=Phải, 2=Xuống, 3=Trái
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
            // Di chuyển random
            int validDirs[4];
            int validCount = 0;
            
            for (int i = 0; i < 4; i++) {
                int newX = e->x + directions[i][0];
                int newY = e->y + directions[i][1];
                
                if (isValidMove(newX, newY) && !isInHistory(e, newX, newY)) {
                    validDirs[validCount++] = i;
                }
            }
            
            if (validCount > 0) {
                int randomDir = validDirs[rand() % validCount];
                addToHistory(e, e->x, e->y);
                e->x += directions[randomDir][0];
                e->y += directions[randomDir][1];
                e->currentDir = randomDir;
                e->facing = randomDir;  // Cập nhật facing
                e->dirCounter = 0;
            }
        } else {
            // Đuổi player
            int bestDir = -1;
            int bestDist = 999;
            
            for (int i = 0; i < 4; i++) {
                int newX = e->x + directions[i][0];
                int newY = e->y + directions[i][1];
                
                if (!isValidMove(newX, newY)) continue;
                
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
                e->facing = bestDir;  // Cập nhật facing
                e->dirCounter = 0;
            }
        }
    }
    // MODE 2: PATROL - Player ở xa
    else {
        e->dirCounter++;

        int bestDir = -1;
        int bestDist = 999;

        for (int i = 0; i < 4; i++)
        {
            int newX = e->x + directions[i][0];
            int newY = e->y + directions[i][1];

            if (!isValidMove(newX, newY)) continue;

            int newDist = getDistance(newX, newY, playerX, playerY);
            if (newDist < bestDist) {
                bestDist = newDist;
                bestDir = i;
            }   
        }
        

        int currentX = e->x + directions[e->currentDir][0];
        int currentY = e->y + directions[e->currentDir][1];
        int curentDist = getDistance(currentX, currentY, playerX, playerY);

        if (isValidMove(currentX, currentY) && curentDist <= bestDist + 1 && e->dirCounter < DIR_CHANGE) {
            addToHistory(e, e->x, e->y);
            e->x = currentX;
            e->y = currentY;
            e->facing = e->currentDir;  // update facing
        } else if (bestDir != -1)
        {
            addToHistory(e, e->x, e->y);
            e->x += directions[bestDir][0];
            e->y += directions[bestDir][1];
            e->currentDir = bestDir;
            e->facing = bestDir;  // update facing
            e->dirCounter = 0;
        }
    }
}

void checkEnemyCollision(Enemy *e, int playerX, int playerY, int playerFacing, int *running) {
    if (!e->alive) return;
    if (e->x != playerX || e->y != playerY) return;

    int opposite = (playerFacing + 2) % 4;
    
    if (e->facing == opposite) {
        // Đối mặt nhau → Player kill enemy
        e->alive = 0;
        // CHÈN ÂM THANH TIÊU DIỆT QUÁI VẬT Ở ĐÂY
        playSuka();

        printf("\033[1;32m[!] Ban da tieu diet 1 quai vat!\033[0m\n");
    } else {
        // CHÈN ÂM THANH PLAYER BỊ QUÁI VẬT KILL Ở ĐÂY
        playPlayerKilled();
        // Không đối mặt → Enemy kill player
        *running = 0;
    }
}
