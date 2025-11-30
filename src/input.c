#include "input.h"

void handleInput(char key, int *x, int *y, int *running) {
    if (key == 'w') (*y)--;
    if (key == 's') (*y)++;
    if (key == 'a') (*x)--;
    if (key == 'd') (*x)++;

    if (key == 'q') *running = 0;
}
