#ifndef RENDER_H
#define RENDER_H

void clearScreen();
void drawMap(int playerX, int playerY, int enemyX, int enemyY, int enemyAlive);
void drawPlayer(int playerX, int playerY);  
void drawEnemy(int enemyX, int enemyY);     

#endif
