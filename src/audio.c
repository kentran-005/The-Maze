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

// Lưu trữ process ID cho các nhạc nền
#ifdef _WIN32
static HANDLE bgMusicHandle = NULL;
static HANDLE menuMusicHandle = NULL;
static HANDLE ingameMusicHandle = NULL;
#else
static pid_t bgMusicPid = -1;      // nhạc nền
static pid_t menuMusicPid = -1;    // nhạc menu
static pid_t ingameMusicPid = -1;  // nhạc trong game
static pid_t victoryMusicPid = -1;
#endif

// ===== KHỞI TẠO VÀ DỌN DẸP HỆ THỐNG AUDIO =====
void initAudio(void) {
    printf("[Audio] System initialized\n");
}

void cleanupAudio(void) {
    stopBackgroundMusic();
    stopMenuSound();
    stopInGame();
    printf("[Audio] System cleaned up\n");
}

// ===== HÀM HỖ TRỢ CHUNG =====
// Hàm tiện ích để phát âm thanh SFX (không loop)
static void playSoundEffect(const char *filename) {
#ifdef _WIN32
    PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
#else
    pid_t sfxPid = fork();
    
    if (sfxPid == 0) {
        // Child process
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) {
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }
        
        #ifdef __APPLE__
        execlp("afplay", "afplay", "-q", "1", filename, NULL);
        #else
        execlp("aplay", "aplay", "-q", filename, NULL);
        #endif
        
        exit(0);
    } else if (sfxPid < 0) {
        printf("[Audio] Warning: Could not fork for sound effect\n");
    }
#endif
}

// ===== NHẠC MENU =====
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
    } else if (menuMusicPid < 0) {
        printf("[Audio] Warning: Could not fork for menu sound\n");
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

// ===== NHẠC NỀN =====
void playBackgroundMusic(void) {
#ifdef _WIN32
    PlaySound("assets/SoundPlaying.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
    bgMusicPid = fork();
    if (bgMusicPid == 0) {
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
    } else if (bgMusicPid < 0) {
        printf("[Audio] Warning: Could not fork for background music\n");
    }
#endif
}

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

// ===== NHẠC TRONG GAME =====
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
    } else if (ingameMusicPid < 0) {
        printf("[Audio] Warning: Could not fork for in-game music\n");
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

void playVictoryMusic(void) {
#ifdef _WIN32
    PlaySound("assets/VictoryN.wav", NULL, SND_FILENAME | SND_ASYNC);
#else
    victoryMusicPid = fork();

    if (victoryMusicPid == 0) {
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) {
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }

        #ifdef __APPLE__
        execlp("afplay", "afplay", "-q", "1", "assets/VictoryN.wav", NULL);
        #else
        execlp("aplay", "aplay", "-q", "assets/VictoryN.wav", NULL);
        #endif

        exit(1);
    } else if (victoryMusicPid < 0) {
        printf("[Audio] Warning: Could not fork for victory music\n");
    }
#endif
}

void stopVictoryMusic(void) {
#ifndef _WIN32
    if (victoryMusicPid > 0) {
        kill(victoryMusicPid, SIGTERM);
        waitpid(victoryMusicPid, NULL, 0);
        victoryMusicPid = -1;
    }
#endif
}

// ===== HIỆU ỨNG ÂM THANH =====
void playLossMusic(void) {
    playSoundEffect("assets/lossmusic.wav");
}

void playDrowningSound(void) {
    playSoundEffect("assets/PlayerKilledFemale.wav");
}

void playPlayerKilled(void) {
    playSoundEffect("assets/DrowningSound.wav");
}

void playSuka(void) {
    playSoundEffect("assets/MonsterKilled.wav");
}

#endif