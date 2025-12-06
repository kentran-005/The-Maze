#ifndef ENEMY_H
#define ENEMY_H

typedef struct {
    int x;
    int y;
    int alive;
} Enemy;

void initEnemy(Enemy *e, int x, int y);
void updateEnemy(Enemy *e, int playerX, int playerY);
void checkEnemyCollision(Enemy *e, int playerX, int playerY, int *running);

#endif
