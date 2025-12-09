#ifndef MAP_H
#define MAP_H
#define MAP_WIDTH 35
#define MAP_HEIGHT 35
extern char gameMap[MAP_HEIGHT][MAP_WIDTH + 1];
extern int playerStartX;
extern int playerStartY;
extern int enemyStartX;
extern int enemyStartY;
extern int exitX;
extern int exitY;
int isWall(int x, int y);
int isExit(int x, int y);
#endif