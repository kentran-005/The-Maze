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
#include <AudioToolbox/AudioToolbox.h>
#endif

#if AUDIO_DISABLED

// Biến lưu trữ process ID cho background music
#ifdef _WIN32
static HANDLE bgMusicHandle = NULL;
static HANDLE menuMusicHandle = NULL;
static HANDLE ingameMusicHandle = NULL;
#else
static pid_t bgMusicPid = -1;      // nhạc nền
static pid_t menuMusicPid = -1;    // nhạc menu
static pid_t ingameMusicPid = -1;  // nhạc trong game
#endif

// ===== KHỞI TẠO VÀ DỌN DẸP HỆ THỐNG AUDIO =====
void initAudio(void) {
    printf("[Audio] System initialized\n");
}

void cleanupAudio(void) {
    stopBackgroundMusic();
    printf("[Audio] System cleaned up\n");
}



void playMenuSound(void) {
#ifndef _WIN32
    menuMusicPid = fork();

    if (menuMusicPid == 0) {
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) {
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }

        #ifdef __APPLE__
        execlp("afplay", "afplay", "-q", "1", "assets/MenuSound.wav", NULL);
        #else
        execlp("aplay", "aplay", "-q", "assets/MenuSound.wav", NULL);
        #endif

        exit(1);
    }
#endif
}

void stopMenuSound(void) {
#ifndef _WIN32
    if (menuMusicPid > 0) {
        kill(menuMusicPid, SIGTERM);
        waitpid(menuMusicPid, NULL, 0);
        menuMusicPid = -1;
    }
#endif
}


// ===== CHÈN CODE PHÁT NHẠC NỀN VÀO ĐÂY =====
void playBackgroundMusic(void) { //x nhac nen
#ifdef _WIN32
    // Windows: Dùng PlaySound (loop)
    PlaySound("assets/SoundPlaying.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
        execlp("afplay", "afplay", "-q", "1", "assets/SoundPlaying.wav", NULL);
        #else
        execlp("aplay", "aplay", "-q", "assets/SoundPlaying.wav", NULL);
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
void playLossMusic(void) { //nhac thua
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
void playVictoryMusic(void) { //nhac winx
   
    const char *filename = "assets/VictoryN.wav"; // thay ten file win

#ifdef _WIN32
    // Windows: Dùng PlaySound
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
#else
    // macOS/Linux: Tạo tiến trình con để chạy afplay/aplay.
    pid_t sfxPid = fork();
    
    if (sfxPid == 0) {
        // Child process
        // Ken: thêm 
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

void playDrowningSound(void) { //nhac duoi nuoc x
   
    const char *filename = "assets/PlayerKilledFemale.wav"; // thay ten file win

#ifdef _WIN32
    // Windows: Dùng PlaySound
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
#else
    // macOS/Linux: Tạo tiến trình con để chạy afplay/aplay.
    pid_t sfxPid = fork();
    
    if (sfxPid == 0) {
        // Child process
        // Ken: thêm 
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

void playPlayerKilled(void) { //nguoi choi bi giet x
    const char *filename = "assets/DrowningSound.wav"; // thay ten file win

#ifdef _WIN32
    // Windows: Dùng PlaySound
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
#else
    // macOS/Linux: Tạo tiến trình con để chạy afplay/aplay.
    pid_t sfxPid = fork();
    
    if (sfxPid == 0) {
        // Child process
        // Ken: thêm 
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

void playSuka(void) { //Quai bi nguoi choi killed x
    
    const char *filename = "assets/MonsterKilled.wav"; // thay ten file win

#ifdef _WIN32
    // Windows: Dùng PlaySound
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
#else
    // macOS/Linux: Tạo tiến trình con để chạy afplay/aplay.
    pid_t sfxPid = fork();
    
    if (sfxPid == 0) {
        // Child process
        // Ken: thêm 
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
void playInGame(void) {
#ifndef _WIN32
    ingameMusicPid = fork();

    if (ingameMusicPid == 0) {
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) {
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }

        #ifdef __APPLE__
        execlp("afplay", "afplay", "-q", "1", "assets/SoundPlaying.wav", NULL);
        #else
        execlp("aplay", "aplay", "-q", "assets/SoundPlaying.wav", NULL);
        #endif

        exit(1);
    }
#endif
}

void stopInGame(void) {
#ifndef _WIN32
    if (ingameMusicPid > 0) {
        kill(ingameMusicPid, SIGTERM);
        waitpid(ingameMusicPid, NULL, 0);
        ingameMusicPid = -1;
    }
#endif
}
// neu can thi stopInGame(){} or k thi th
#endif