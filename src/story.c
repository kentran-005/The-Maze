#include "story.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// saveProgress function

#ifdef _WIN32
    #include <windows.h>
    #define sleep_ms(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define sleep_ms(ms) usleep((ms) * 1000)
#endif

#define PROGRESS_FILE "assets/progress.dat"



// File reading function

void playStoryFromFile(const char *filename, int delayMs) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("[ERROR] Khong tim thay file: %s\n", filename);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        typeText(line, delayMs);
        sleep_ms(100);
    }

    fclose(f);
}

// Load and display story from file
void loadStory() {
    FILE *f = fopen("assets/story.txt", "r");
    if (!f) return;

    char line[200];
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
        usleep(800000);
    }

    printf("\n\nNhan Enter de bat dau game...");
    getchar();
}


// Load/save progress

void loadProgress(GameProgress *progress) {
    FILE *f = fopen(PROGRESS_FILE, "rb");
    if (f) {
        // File exists - read progress
        if (fread(progress, sizeof(GameProgress), 1, f) != 1) {
            // fread failed, initialize with defaults
            progress->easyCompleted = 0;
            progress->normalCompleted = 0;
            progress->hardCompleted = 0;
        }
        fclose(f);
    } else {
        // File not exist, initialize progress
        progress->easyCompleted = 0;
        progress->normalCompleted = 0;
        progress->hardCompleted = 0;
    }
}

void saveProgress(GameProgress *progress) {
    FILE *f = fopen(PROGRESS_FILE, "wb");
    if (!f) {
        printf("[ERROR] Khong ghi duoc file progress!\n");
        return;
    }
    fwrite(progress, sizeof(GameProgress), 1, f);
    fclose(f);
    printf("\nProgress saved.\n");
}

// Typing effect for text display

void typeText(const char *text, int delayMs) {
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
        sleep_ms(delayMs);
    }
}

void showStoryEasy(void) {
    printf("\n");
    printf("+===============================================+\n");
    printf("|          CHUONG I: xxxxxxxxx                  |\n");
    printf("+===============================================+\n\n");
    
    sleep_ms(1000);
    // playStoryFromFile("story_easy.txt", 30);

    printf("+===============================================+\n");
    printf("|          CHE DO TRUNG BINH DA MO              |\n");
    printf("+===============================================+\n\n");

    printf("\nNhan phim bat ky de tiep tuc...");
    getchar();
}

void showStoryNormal(void) {
    printf("\n");
    printf("+===============================================+\n");
    printf("|          CHUONG II: xxxxxxxxx                 |\n");
    printf("+===============================================+\n\n");
    
    sleep_ms(1000);
    // playStoryFromFile("story_normal.txt", 30);

    printf("+===============================================+\n");
    printf("|            CHE DO KHO DA MO                   |\n");
    printf("+===============================================+\n\n");

    printf("\nNhan phim bat ky de tiep tuc...");
    getchar();
}

void showStoryHard(void) {
    printf("\n");
    printf("+===============================================+\n");
    printf("|          CHUONG III: xxxxxxxxx                |\n");
    printf("+===============================================+\n\n");
    
    sleep_ms(1000);
    // playStoryFromFile("story_hard.txt", 30);

    printf("+===============================================+\n");
    printf("|      BAN DA HOAN THANH TAT CA CHE DO          |\n");
    printf("+===============================================+\n\n");

    printf("\nNhan phim bat ky de tiep tuc...");
    getchar();
}

// 

int checkSecretEnding (GameProgress *progress) {
    return (progress->easyCompleted && progress->normalCompleted && progress->hardCompleted);
}

void showSecretEnding (void) {
    printf("\n");
    printf("+===============================================+\n");
    printf("|                                               |\n");
    printf("|          Secret Ending.                       |\n");
    printf("|                                               |\n");
    printf("+===============================================+\n\n");
    
    sleep_ms(2000);
    // playStoryFromFile("story_hard.txt", 30);

    printf("+===============================================+\n");
    printf("|                                               |\n");
    printf("|          CHUC MUNG! BAN DA HOAN THANH.        |\n");
    printf("|                    THE-MAZE                   |\n");
    printf("|                                               |\n");
    printf("|               TRUE ENDING UNLOCKED?           |\n");
    printf("|                                               |\n");
    printf("+===============================================+\n\n");

    sleep_ms(2000);

    printf(" 👑👑👑 THANK YOU FOR PLAYING 👑👑👑 \n\n");

    printf("\nNhan phim bat ky de tiep tuc...");
    getchar();
}

// MENU PROGRESS

void showProgressMenu(GameProgress *progress) {
    printf("\n");
    printf("+===============================================+\n");
    printf("|              TIEN DO HOAN THANH.              |\n");
    printf("+===============================================+\n\n");

    printf("  [%s] Mode De\n", 
           progress->easyCompleted ? "\033[1;32m✓\033[0m" : " ");
    printf("  [%s] Mode Trung Binh\n", 
           progress->normalCompleted ? "\033[1;32m✓\033[0m" : " ");
    printf("  [%s] Mode Kho\n", 
           progress->hardCompleted ? "\033[1;32m✓\033[0m" : " ");
    
    printf("\n");
    
    if (checkSecretEnding(progress)) {
        printf("  \033[1;33m[★] SECRET ENDING UNLOCKED!\033[0m\n\n");
    } else {
        int remaining = 3;
        if (progress->easyCompleted) remaining--;
        if (progress->normalCompleted) remaining--;
        if (progress->hardCompleted) remaining--;
        printf("  Con lai %d mode de mo khoa Secret Ending...\n\n", remaining);
    }
}