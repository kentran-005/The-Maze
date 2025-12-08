#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

static int kbhit_posix(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return 0;
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();
    if (ch != EOF) {
        ungetc(ch, stdin);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        return 1;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return 0;
}

static int getch_posix(void) {
    struct termios oldt, newt;
    int ch;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return EOF;
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

#define _kbhit() kbhit_posix()
#define _getch() getch_posix()
#define Sleep(ms) usleep((ms) * 1000)
#endif

#include "game.h"
#include "render.h"
#include "input.h"
#include <time.h>
#include "enemy.h"
#include "map.h"

#define MAX_OXY 100
#define TIME_LIMIT 180
#define MAX_ENEMIES 4

int playerFacing = 1; // 0=left, 1=right

void drawOxyBar(int oxy) {
    int bars = 20;
    int filled = (oxy * bars) / 100;

    if (filled < 0) filled = 0;
    if (filled > bars) filled = bars;

    printf("OXY: [");
    for (int i = 0; i < bars; i++) {
        if (i < filled) printf("#");
        else printf("-");
    }
    printf("] %d%%\n", oxy);

    if (oxy > 0 && oxy <= 20) {
        printf("\033[1;31mCANH BAO: SAP HET OXY!\033[0m\n");
    }
}

int selectDifficulty() {
    clearScreen();
    printf("\n");
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║       CHON DO KHO - SELECT DIFFICULTY     ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    printf("\n");
    printf("  1. DE (EASY)           - 1 Quai vat\n");
    printf("  2. TRUNG BINH (NORMAL) - 2 Quai vat\n");
    printf("  3. KHO (HARD)          - 4 Quai vat\n");
    printf("\n");
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║  Nhap lua chon (1-3): ");

    char choice;
    while (1) {
        choice = getchar();
        if (choice == '1' || choice == '2' || choice == '3') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return choice - '0';
        }
    }
}

void startGame() {
    // Enable UTF-8 on Windows
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
    #endif

    int difficulty = selectDifficulty();
    int numEnemies = 0;

    switch(difficulty) {
        case 1: numEnemies = 1; break;
        case 2: numEnemies = 2; break;
        case 3: numEnemies = 4; break;
        default: numEnemies = 1;
    }

    int running = 1;
    int x = playerStartX;
    int y = playerStartY;
    playerFacing = 1; // Start facing right
    
    int oxy = MAX_OXY;
    time_t startTime = time(NULL);
    
    srand((unsigned int)time(NULL));

    // Khoi tao mang enemies
    Enemy enemies[MAX_ENEMIES];
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].alive = 0; // Mac dinh tat ca chua spawn
    }

    // Vi tri spawn
    int spawnPositions[4][2] = {
        {3, 3},
        {16, 3},
        {3, 16},
        {16, 16}
    };

    // Spawn enemies theo do kho
    for (int i = 0; i < numEnemies; i++) {
        initEnemy(&enemies[i], spawnPositions[i][0], spawnPositions[i][1]);
    }

    // Man hinh bat dau
    clearScreen();
    printf("\n╔═══════════════════════════════════════════╗\n");
    if (difficulty == 1) {
        printf("║     CHE DO: DE (1 QUAI VAT)              ║\n");
    } else if (difficulty == 2) {
        printf("║     CHE DO: TRUNG BINH (2 QUAI VAT)      ║\n");
    } else {
        printf("║     CHE DO: KHO (4 QUAI VAT)             ║\n");
    }
    printf("╚═══════════════════════════════════════════╝\n");
    printf("\nNhan phim bat ky de bat dau...\n");
    _getch();

    // GAME LOOP
    while (running) {
        int elapsed = (int)(time(NULL) - startTime);

        // Tinh OXY
        oxy = MAX_OXY - (elapsed * MAX_OXY / TIME_LIMIT);
        if (oxy < 0) oxy = 0;

        // Het OXY
        if (oxy <= 0) {
            clearScreen();
            printf("\n╔═══════════════════════════════════════╗\n");
            printf("║        HET OXY - GAME OVER            ║\n");
            printf("╚═══════════════════════════════════════╝\n");
            printf("\nBAN DA HET OXY TRONG ME CUNG!\n");
            printf("Thoi gian song: %d giay\n\n", elapsed);

            #ifdef _WIN32
            Sleep(3000);
            #else
            usleep(3000000);
            #endif
            break;
        }

        // Cap nhat TAT CA enemies
        for (int i = 0; i < numEnemies; i++) {
            if (enemies[i].alive) {
                updateEnemy(&enemies[i], x, y);
                checkEnemyCollision(&enemies[i], x, y, playerFacing, &running);

                if (!running) {
                    clearScreen();
                    printf("\n╔═══════════════════════════════════════╗\n");
                    printf("║     QUAI VAT - GAME OVER              ║\n");
                    printf("╚═══════════════════════════════════════╝\n");
                    printf("\nBAN DA BI QUAI VAT SO %d BAT!\n", i + 1);
                    printf("Thoi gian song: %d giay\n\n", elapsed);

                    #ifdef _WIN32
                    Sleep(3000);
                    #else
                    usleep(3000000);
                    #endif
                    break;
                } 
            } else {
                updateEnemyRespawn(&enemies[i], x, y); // Cap nhat respawn
            }
        }

        if (!running) break;

        // RENDER - CHI 1 LAN DUY NHAT
        clearScreen();
        printf("╔═══════════════════════════════════════╗\n");
        printf("║      ESCAPE THE MAZE GAME             ║\n");
        printf("╚═══════════════════════════════════════╝\n");
        printf("Thoi gian: %d/%d giay | Quai vat: %d\n\n",
               elapsed, TIME_LIMIT, numEnemies);

        drawOxyBar(oxy);
        printf("\n");

        drawMapWithMultipleEnemies(x, y, enemies, numEnemies, playerFacing);

        printf("\n╔═══════════════════════════════════════╗\n");
        printf("║ W/A/S/D: Di chuyen | Q: Thoat        ║\n");
        printf("║ Muc tieu: Tim loi thoat (E)!         ║\n");
        printf("╚═══════════════════════════════════════╝\n");

        fflush(stdout); // Dam bao output duoc in ra ngay

        // XU LY INPUT
        if (_kbhit()) {
            char key = _getch();
            handleInput(key, &x, &y, &running, &playerFacing);

            // Kiem tra WIN
            if (isExit(x, y)) {
                clearScreen();
                printf("\n╔═══════════════════════════════════════╗\n");
                printf("║       CHIEN THANG! 🎉                ║\n");
                printf("╚═══════════════════════════════════════╝\n");
                printf("\nBan da thoat khoi me cung!\n");
                printf("Do kho: ");
                if (difficulty == 1) printf("DE\n");
                else if (difficulty == 2) printf("TRUNG BINH\n");
                else printf("KHO\n");
                printf("Thoi gian: %d giay\n", elapsed);
                printf("OXY con lai: %d%%\n\n", oxy);

                #ifdef _WIN32
                Sleep(3000);
                #else
                usleep(3000000);
                #endif

                running = 0;
            }
        }

        // Delay cho game muot
        #ifdef _WIN32
        Sleep(200);
        #else
        usleep(100000);
        #endif
    }
}