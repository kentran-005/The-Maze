#ifndef INPUT_H
#define INPUT_H

#ifdef _WIN32
    #include <conio.h>
#else
    int _kbhit();
    int _getch();
#endif

void handleInput(char key, int *x, int *y, int *running, int *playerFacing);

#endif
