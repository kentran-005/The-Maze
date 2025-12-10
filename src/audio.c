#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#else
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#endif

#if AUDIO_DISABLED

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
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) {
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }
        #ifdef __APPLE__
        execlp("afplay", "afplay", "-q", "1", "assets/background.wav", NULL);
        #else
        execlp("aplay", "aplay", "-q", "assets/background.wav", NULL);
        #endif
        exit(1);
    }else if (bgMusicPid < 0) {
        printf("[Audio] Warning: Could not fork for background music\n");
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
        waitpid(bgMusicPid, NULL, 0);
        bgMusicPid = -1;
    }
#endif
}

//vua them 2h49:->
void playLossMusic(void) {
    const char *filename = "assets/lossmusic.wav"; //
#ifdef _WIN32
    // Windows: Dùng PlaySound
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC); // hieu ung tieu diet quai vat
#else
    // macOS/Linux: Tạo tiến trình con để chạy afplay/aplay.
    pid_t sfxPid = fork();
    
    // Logic chỉ chạy nếu fork() thành công và đây là tiến trình con.
    if (sfxPid == 0) {
        // Child process
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) {
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }
        
        #ifdef __APPLE__
        // macOS: afplay
        execlp("afplay", "afplay", "-q", "1", filename, NULL);
        #else
        // Linux: aplay
        execlp("aplay", "aplay", "-q", filename, NULL);
        #endif
        
        // Thoát tiến trình con nếu execlp thất bại
        exit(0); 
    }

#endif
}

//vua them 
void playVictoryMusic(void) {
   
    const char *filename = "assets/winmusic.wav"; // thay ten file win

#ifdef _WIN32
    // Windows: Dùng PlaySound
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
#else
    // macOS/Linux: Tạo tiến trình con để chạy afplay/aplay.
    pid_t sfxPid = fork();
    
    if (sfxPid == 0) {
        // Child process
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) {
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }
        
        #ifdef __APPLE__
        // macOS: afplay
        execlp("afplay", "afplay", "-q", "1", filename, NULL);
        #else
        // Linux: aplay
        execlp("aplay", "aplay", "-q", filename, NULL);
        #endif
        
        // Thoát tiến trình con nếu execlp thất bại
        exit(0); 
    }

#endif
}

#endif