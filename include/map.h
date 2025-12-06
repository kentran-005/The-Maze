#ifndef MAP_H
#define MAP_H
#define MAP_WIDTH 20
#define MAP_HEIGHT 20
extern char gameMap[MAP_HEIGHT][MAP_WIDTH];
extern int playerStartX;
extern int playerStartY;
extern int enemyStartX;
extern int enemyStartY;
extern int exitX;
extern int exitY;
int isWall(int x, int y);
int isExit(int x, int y);
#endif