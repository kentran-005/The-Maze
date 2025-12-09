#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

#include "game.h"
#include "render.h"
#include "input.h"
#include "enemy.h"
#include "map.h"


#define MAX_OXY 100
#define TIME_LIMIT 180
#define MAX_ENEMIES 11

int playerFacing = 1; // 0=left, 1=right

#ifndef _WIN32
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

static void enterFullscreenConsole() {
    #ifdef _WIN32
    // Enable UTF-8 on Windows
    SetConsoleOutputCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
    #endif

    printf("\033[?1049h"); // Switch to alternate buffer
    printf("\033[?25l");   // Hide cursor
    printf("\033[2J\033[H"); // Clear screen and home cursor
    fflush(stdout);
}

static void leaveFullscreenConsole() {
    printf("\033[?1049l");   // Show cursor
    printf("\033[?25h"); // Switch back from alternate buffer
    fflush(stdout);
}



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
    // playMenuSound(); // CHÈN CODE PHÁT NHẠC MENU Ở ĐÂY
    printf("\033[2J\033[H");
    fflush(stdout);

    printf("\n");
    printf("+===========================================+\n");
    printf("|       CHON DO KHO - SELECT DIFFICULTY     |\n");
    printf("+===========================================+\n");
    printf("\n");
    printf("  1. DE (EASY)           - 3 Quai vat\n");
    printf("  2. TRUNG BINH (NORMAL) - 7 Quai vat\n");
    printf("  3. KHO (HARD)          - 11 Quai vat (!)\n");
    printf("\n");
    printf("     *** CHU Y: Che do KHO cuc ky kho! ***\n");
    printf("\n");
    printf("+===========================================+\n");
    printf("|  Nhap lua chon (1-3): ");

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

    enterFullscreenConsole();

    int difficulty = selectDifficulty();
    int numEnemies = 0;

    switch(difficulty) {
        case 1: numEnemies = 3; break;
        case 2: numEnemies = 7; break;
        case 3: numEnemies = 11; break;
        default: numEnemies = 3;
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
   int spawnPositions[11][2] = {
    {7, 3},     // Góc trên trái
    {10, 2},    // Trên giữa
    {28, 3},    // Trên phải

    {5, 10},    // Trái giữa
    {17, 9},    // Trung tâm trên
    {29, 11},   // Phải giữa

    {4, 20},    // Trái dưới
    {17, 18},   // Trung tâm giữa
    {30, 18},   // Phải dưới

    {10, 30},   // Dưới trái
    {22, 30}    // Dưới phải
};

    // Spawn enemies theo do kho
    for (int i = 0; i < numEnemies; i++) {
        initEnemy(&enemies[i], spawnPositions[i][0], spawnPositions[i][1]);
    }

    // Man hinh bat dau
    printf("\033[2J\033[H");
      printf("\n+===========================================+\n");
    if (difficulty == 1) {
        printf("|     CHE DO: DE (3 QUAI VAT)              |\n");
    } else if (difficulty == 2) {
        printf("|     CHE DO: TRUNG BINH (7 QUAI VAT)      |\n");
    } else {
        printf("|     CHE DO: KHO (11 QUAI VAT - DIA NGUC!)|\n");
    }
    printf("+===========================================+\n");
    printf("\nNhan phim bat ky de bat dau...\n");
    _getch();

    // tracking state de render toi thieu
    int lastPlayerX = -1, lastPlayerY = -1, lastPlayerFacing = -1;
    int lastOxy = -1, lastElapsed = -1;
    int lastEnemyPos[MAX_ENEMIES][2];
    for (int i = 0; i < MAX_ENEMIES; i++) { lastEnemyPos[i][0] = -1; lastEnemyPos[i][1] = -1; }

    int needsRender = 1;

    // GAME LOOP
    while (running) {
        int elapsed = (int)(time(NULL) - startTime);

        // Tinh OXY
        oxy = MAX_OXY - (elapsed * MAX_OXY / TIME_LIMIT);
        if (oxy < 0) oxy = 0;

        // Het OXY
        if (oxy <= 0) {
            // DỪNG NHẠC NỀN

            // CHÈN NHẠC LOSS KHI HẾT OXY Ở ĐÂY

            clearScreen();
            printf("\n╔═══════════════════════════════════════╗\n");
            printf("║        HET OXY - GAME OVER            ║\n");
            printf("╚═══════════════════════════════════════╝\n");
            printf("\nBAN DA HET OXY TRONG ME CUNG!\n");
            printf("Thoi gian song: %d giay\n\n", elapsed);

            Sleep(2000);
            break;
            // #ifdef _WIN32
            // Sleep(3000);
            // #else
            // sleep(2);
            // #endif
            // break;
        }

        // Cap nhat TAT CA enemies
        for (int i = 0; i < numEnemies; i++) {
            if (enemies[i].alive) {
                updateEnemy(&enemies[i], x, y);
                checkEnemyCollision(&enemies[i], x, y, playerFacing, &running);

                if (!running) {
                    // CHỈ GỌI MỖI HÀM DỪNG NHẠC NỀN Ở ĐÂY

                    clearScreen();
                    printf("\n╔═══════════════════════════════════════╗\n");
                    printf("║     QUAI VAT - GAME OVER              ║\n");
                    printf("╚═══════════════════════════════════════╝\n");
                    printf("\nBAN DA BI QUAI VAT SO %d BAT!\n", i + 1);
                    printf("Thoi gian song: %d giay\n\n", elapsed);

                    Sleep(2000);
                    break;
                    // #ifdef _WIN32
                    // Sleep(3000);
                    // #else
                    // sleep(2);
                    // #endif
                    // break;
                } 
            } else {
                updateEnemyRespawn(&enemies[i], x, y); // Cap nhat respawn
            }
        }

        if (!running) break;

        // Add logic de chi render khi can thiet
        int hasChanged = 0;
        if (x != lastPlayerX || y != lastPlayerY ||playerFacing != lastPlayerFacing)
        {
            hasChanged = 1;
            lastPlayerX = x; lastPlayerY = y; lastPlayerFacing = playerFacing;  
        }

        if (oxy != lastOxy || elapsed != lastElapsed)
        {
            hasChanged = 1;
            lastOxy = oxy; lastElapsed = elapsed;
        }

        for (int i = 0; i < numEnemies; i++)
        {
            int ex = (enemies[i].alive ? enemies[i].x : -1);
            int ey = (enemies[i].alive ? enemies[i].y : -1);
            if (ex != lastEnemyPos[i][0] || ey != lastEnemyPos[i][1])
            {
                hasChanged = 1;
                lastEnemyPos[i][0] = ex;
                lastEnemyPos[i][1] = ey;
            }
        }
        
        if (!hasChanged || !needsRender) {
            needsRender = 0;
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

            fflush(stdout); 
        }

        // XU LY INPUT
        if (_kbhit()) {
            char key = _getch();
            handleInput(key, &x, &y, &running, &playerFacing);

            // Kiem tra Victory
            if (isExit(x, y)) {
                // DỪNG NHẠC NỀN Ở ĐÂY

                // CHÈN NHẠC VICTORY Ở ĐÂY

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

                Sleep(2000);
                break;
                // #ifdef _WIN32
                // Sleep(3000);
                // #else
                // sleep(2);
                // #endif

                running = 0;
            }
        }

        Sleep(120);
        // Delay cho game muot
        // #ifdef _WIN32
        // Sleep(100);
        // #else
        // usleep(100000);
        // #endif
    }
    leaveFullscreenConsole();
}