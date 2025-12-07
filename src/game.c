#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

static int kbhit_posix(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    if (tcgetattr(STDIN_FILENO, &oldt) == -1)
        return 0;
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        return 1;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return 0;
}

static int getch_posix(void)
{
    struct termios oldt, newt;
    int ch;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1)
        return EOF;
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
#define TIME_LIMIT 240
#define MAX_ENEMIES 4

void drawOxyBar(int oxy)
{
    int bars = 20;
    int filled = oxy * bars / 100;

    if (filled < 0)
        filled = 0;
    if (filled > bars)
        filled = bars;

    printf("OXY: [");
    for (int i = 0; i < bars; i++)
    {
        if (i < filled)
            printf("#");
        else
            printf("-");
    }
    printf("] %d%%\n", oxy);

    if (oxy > 0 && oxy <= 20)
    {
        printf("\033[1;31mCANH BAO: SAP HET OXY!\n\033[0m");
    }
}

int selectDifficulty()
{
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
    while (1)
    {
        choice = getchar();
        if (choice == '1' || choice == '2' || choice == '3')
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            return choice - '0';
        }
    }
}

void startGame()
{
    int difficulty = selectDifficulty();
    int numEnemies = 0;

    switch (difficulty)
    {
    case 1:
        numEnemies = 1;
        break;
    case 2:
        numEnemies = 2;
        break;
    case 3:
        numEnemies = 4;
        break;
    default:
        numEnemies = 1;
    }
    time_t startTime = time(NULL);
    int elapsed = 0;
    int prevElapsed = -1;
    int oxy = MAX_OXY;
    int running = 1;

    int x = playerStartX;
    int y = playerStartY;

    srand((unsigned int)time(NULL));

    Enemy enemies[MAX_ENEMIES];

    int spawnPositions[4][2] = {
        {15, 8}, // Vị trí cũ của enemy trong map.c → an toàn
        {8, 14}, // Góc dưới trái
        {12, 4}, // Trên giữa
        {17, 12} // Gần exit → tăng độ khó
    };

    for (int i = 0; i < numEnemies; i++)
    {
        initEnemy(&enemies[i], spawnPositions[i][0], spawnPositions[i][1]);
    }

    int needsRedraw = 1;
    int enemyMoved = 0;

    clearScreen();
    printf("\n╔═══════════════════════════════════════════╗\n");
    if (difficulty == 1)
    {
        printf("║     CHE DO: DE (1 QUAI VAT)              ║\n");
    }
    else if (difficulty == 2)
    {
        printf("║     CHE DO: TRUNG BINH (2 QUAI VAT)      ║\n");
    }
    else
    {
        printf("║     CHE DO: KHO (4 QUAI VAT)             ║\n");
    }
    printf("╚═══════════════════════════════════════════╝\n");
    printf("\nNhan phim bat ky de bat dau...\n");
    _getch();
    clearScreen();

#ifdef _WIN32
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
        SetConsoleMode(hOut, dwMode);
        SetConsoleOutputCP(65001);
    }
#endif
    elapsed = (int)(time(NULL) - startTime);

    while (running)
    {
        // Cập nhật thời gian thực (wall time)

        // Giảm oxy mỗi khi qua 1 giây mới
        if (elapsed > prevElapsed)
        {
            prevElapsed = elapsed;
            oxy--;
            needsRedraw = 1;
        }

        // Hết oxy → Game Over
        if (oxy <= 0)
        {
            oxy = 0;
            clearScreen();
            printf("\n╔═══════════════════════════════════════╗\n");
            printf("║        HET OXY - GAME OVER            ║\n");
            printf("╚═══════════════════════════════════════╝\n");
            printf("\nBAN DA HET OXY TRONG ME CUNG!\n");
            printf("Thoi gian song sot: %d giay\n\n", elapsed);

#ifdef _WIN32
            Sleep(4000);
#else
            usleep(4000000);
#endif
            break;
        }

        // Hết thời gian giới hạn → Game Over
        if (elapsed >= TIME_LIMIT)
        {
            clearScreen();
            printf("\n╔═══════════════════════════════════════╗\n");
            printf("║       HET THOI GIAN - GAME OVER       ║\n");
            printf("╚═══════════════════════════════════════╝\n");
            printf("\nBAN DA HET THOI GIAN QUY DINH!\n");
            printf("Oxy con lai: %d%%\n\n", oxy);

#ifdef _WIN32
            Sleep(4000);
#else
            usleep(4000000);
#endif
            break;
        }

        // === CẬP NHẬT ENEMY ===
        enemyMoved = 0;
        for (int i = 0; i < numEnemies; i++)
        {
            if (enemies[i].alive)
            {
                int oldX = enemies[i].x;
                int oldY = enemies[i].y;
                updateEnemy(&enemies[i], x, y);

                if (enemies[i].x != oldX || enemies[i].y != oldY)
                {
                    enemyMoved = 1;
                }

                checkEnemyCollision(&enemies[i], x, y, &running);
                if (!running)
                {
                    clearScreen();
                    printf("\n╔═══════════════════════════════════════╗\n");
                    printf("║     QUAI VAT DA BAT BAN!              ║\n");
                    printf("╚═══════════════════════════════════════╝\n");
                    printf("\nBAN DA BI QUAI VAT SO %d BAT!\n", i + 1);
                    printf("Thoi gian song sot: %d giay\n", elapsed);
                    printf("Oxy con lai: %d%%\n\n", oxy);

#ifdef _WIN32
                    Sleep(4000);
#else
                    usleep(4000000);
#endif
                    break;
                }
                if (!running)
                    break;
            }
        }

        if (enemyMoved || _kbhit())
        {
            if (_kbhit())
            {
                char key = _getch();
                int oldX = x, oldY = y;
                handleInput(key, &x, &y, &running);
                if (x != oldX || y != oldY)
                    needsRedraw = 1;
                if (isExit(x, y))
                { /* thắng */
                    running = 0;
                }
            }
            needsRedraw = 1;
        }
        if (elapsed > prevElapsed)
        {
            prevElapsed = elapsed;
            oxy--;
            needsRedraw = 1;
        }

        // === VẼ LẠI MÀN HÌNH ===
        if (needsRedraw)
        {
            clearScreen();
            printf("╔═══════════════════════════════════════════════════════╗\n");
            printf("║         ESCAPE THE MAZE - SURVIVE!                    ║\n");
            printf("╚═══════════════════════════════════════════════════════╝\n");
            printf("Thoi gian: %3d/%d giay  |  Quai vat: %d  |  Oxy: %d%%\n\n",
                   elapsed, TIME_LIMIT, numEnemies, oxy);

            drawOxyBar(oxy);
            printf("\n");

            drawMapWithMultipleEnemies(x, y, enemies, numEnemies);

            printf("\n╔═══════════════════════════════════════════════════╗\n");
            printf("║   W/A/S/D = Di chuyen     Q = Thoat game        ║\n");
            printf("║         Tim chu 'E' de thoat khoi me cung!      ║\n");
            printf("╚═══════════════════════════════════════════════════╝\n");

            needsRedraw = 0; // Quan trọng: reset cờ sau khi vẽ
            fflush(stdout);
        }
        // === XỬ LÝ INPUT ===
        if (_kbhit())
        {
            char key = _getch();
            int oldX = x;
            int oldY = y;

            handleInput(key, &x, &y, &running);

            if (x != oldX || y != oldY)
            {
                needsRedraw = 1; // Player di chuyển → vẽ lại
            }

            // Kiểm tra thắng
            if (isExit(x, y))
            {
                clearScreen();
                printf("\n╔═══════════════════════════════════════╗\n");
                printf("║           BAN DA THANG!               ║\n");
                printf("╚═══════════════════════════════════════════════════════╝\n");
                printf("\nCHUC MUNG! BAN DA THOAT KHOI ME CUNG!\n");
                printf("Do kho: ");
                if (difficulty == 1)
                    printf("DE\n");
                else if (difficulty == 2)
                    printf("TRUNG BINH\n");
                else
                    printf("KHO\n");
                printf("Thoi gian hoan thanh: %d giay\n", elapsed);
                printf("Oxy con lai: %d%%\n\n", oxy);

#ifdef _WIN32
                Sleep(6000);
#else
                usleep(6000000);
#endif
                running = 0;
            }
        }

// Delay nhỏ để game mượt, không ăn CPU
#ifdef _WIN32
        Sleep(200);
#else
        usleep(2000000);
#endif
    }
}
