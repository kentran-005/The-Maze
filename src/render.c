#include <stdio.h>
#include "render.h"

void clearScreen() {
    printf("\033[2J\033[1;1H");
}

void drawPlayer(int x, int y) {
    for (int i = 0; i < y; i++) printf("\n");
    for (int i = 0; i < x; i++) printf(" ");
    printf("@");
}
