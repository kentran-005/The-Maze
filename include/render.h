#ifndef RENDER_H
#define RENDER_H

#include "enemy.h" 

void clearScreen();
void drawMap(int playerX, int playerY, int enemyX, int enemyY, int enemyAlive);
void drawMapWithMultipleEnemies(int playerX, int playerY, Enemy *enemies, int numEnemies); 
void drawPlayer(int playerX, int playerY);
void drawEnemy(int enemyX, int enemyY);  

#endif
