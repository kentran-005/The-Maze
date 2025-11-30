#include "audio.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

void play_music(const char* filename) {
#ifdef _WIN32
    PlaySound(filename, NULL, SND_ASYNC | SND_FILENAME);
#else
    char command[256];
    sprintf(command, "afplay \"%s\" &", filename);
    system(command);
#endif
}

void playBackgroundMusic(void) {
    // file music
    play_music("assets/background.wav");
}

void stopBackgroundMusic(void) {
#ifdef _WIN32
    PlaySound(NULL, NULL, 0); // Dừng nhạc trên Windows
#else
    system("killall afplay 2>/dev/null"); // Dừng afplay trên macOS
#endif
}