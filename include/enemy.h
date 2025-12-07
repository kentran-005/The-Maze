#ifndef ENEMY_H
#define ENEMY_H

#define HISTORY_SIZE 5

typedef struct {
    int x, y;
    int alive;
    
    int moveCounter;
    int currentDir;
    int dirCounter;
    int historyX[HISTORY_SIZE];
    int historyY[HISTORY_SIZE];
    int historyIndex;
} Enemy;

void initEnemy(Enemy *e, int x, int y);
void updateEnemy(Enemy *e, int playerX, int playerY);
void checkEnemyCollision(Enemy *e, int playerX, int playerY, int *running);

#endif