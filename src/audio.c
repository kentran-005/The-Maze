#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#else
#include <unistd.h>
#include <signal.h>
#endif

// Biến lưu trữ process ID cho background music
#ifdef _WIN32
static HANDLE bgMusicHandle = NULL;
#else
static pid_t bgMusicPid = -1;
#endif

// ===== KHỞI TẠO VÀ DỌN DẸP HỆ THỐNG AUDIO =====
void initAudio(void) {
    printf("[Audio] System initialized\n");
}

void cleanupAudio(void) {
    stopBackgroundMusic();
    printf("[Audio] System cleaned up\n");
}

// ===== CHÈN CODE PHÁT NHẠC NỀN VÀO ĐÂY =====
void playBackgroundMusic(void) {
#ifdef _WIN32
    // Windows: Dùng PlaySound (loop)
    PlaySound("assets/background.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
    // macOS/Linux: Dùng afplay (Mac) hoặc aplay (Linux)
    bgMusicPid = fork();
    if (bgMusicPid == 0) {
        // Child process
        #ifdef __APPLE__
        execlp("afplay", "afplay", "-q", "1", "assets/background.wav", "--loop", "0", NULL);
        #else
        execlp("aplay", "aplay", "-q", "assets/background.wav", NULL);
        #endif
        exit(1);
    }
#endif
}
// Dừng nhạc nền
void stopBackgroundMusic(void) {
#ifdef _WIN32
    PlaySound(NULL, 0, 0);
#else
    if (bgMusicPid > 0) {
        kill(bgMusicPid, SIGTERM);
        bgMusicPid = -1;
    }
#endif
}

