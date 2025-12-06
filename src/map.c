/* ===========================
   FILE: map.c
   =========================== */
#include "map.h"
#include <stdio.h>
#include <stdlib.h>

char gameMap[MAP_HEIGHT][MAP_WIDTH] = {
    "####################",
    "#        #         #",
    "# ##### ## ####### #",
    "# #   # #  #     # #",
    "# # # # ## # ### # #",
    "# # # #    # #   # #",
    "# # # #### # # ### #",
    "# # #      # #   # #",
    "# # ######## ### # #",
    "# #          #   # #",
    "# ########## # ### #",
    "#          # #   # #",
    "# ####### ## ### # #",
    "# #     # #    # # #",
    "# # ### # #### # # #",
    "# #   # #      #   #",
    "# ### # ######## ##E",
    "#   #            # #",
    "### ############## #",
    "####################"
};

// Vị trí spawn player
int playerStartX = 1;
int playerStartY = 1;

// Vị trí spawn enemy
int enemyStartX = 15;
int enemyStartY = 8;

// Vị trí exit
int exitX = 18;
int exitY = 16;

// Kiểm tra tường
int isWall(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return 1; // Ngoài map = tường
    }
    return gameMap[y][x] == '#';
}

// Kiểm tra exit
int isExit(int x, int y) {
    return (x == exitX && y == exitY);
}

