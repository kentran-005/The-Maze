#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "audio.h"

// Hàm tiện ích xóa màn hình
static void clearConsoleScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Hàm tiện ích hiển thị menu
static void displayMainMenu(void) {
    clearConsoleScreen();
    printf("\n");
    printf("╔═══════════════════════════════════════╗\n");
    printf("║    ESCAPE THE MAZE - MAIN MENU        ║\n");
    printf("║                                       ║\n");
    printf("║    Ban da san sang khong?             ║\n");
    printf("║    Are you ready?                     ║\n");
    printf("║                                       ║\n");
    printf("║    Nhan phim bat ky de bat dau...    ║\n");
    printf("║    Press any key to start...          ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    printf("\n");
}

// Hàm hỏi chơi lại
static int askPlayAgain(void) {
    char choice;
    
    do {
        printf("\n╔═══════════════════════════════════════╗\n");
        printf("║  Ban co muon choi lai khong?          ║\n");
        printf("║  Do you want to play again?           ║\n");
        printf("╚═══════════════════════════════════════╝\n");
        printf("Nhap (y/n): ");
        
        scanf(" %c", &choice);

        // Xóa input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // Kiểm tra input hợp lệ
        if (choice == 'y' || choice == 'Y') {
            return 1;  // Chơi lại
        } else if (choice == 'n' || choice == 'N') {
            return 0;  // Thoát
        } else {
            printf("\033[1;31mLua chon khong hop le, vui long chon lai (y/n)!\033[0m\n");
        }

    } while (1);
}

// Hàm hiển thị màn hình kết thúc
static void displayEndScreen(void) {
    clearConsoleScreen();
    printf("\n");
    printf("╔═══════════════════════════════════════╗\n");
    printf("║     CAM ON DA CHOI GAME!              ║\n");
    printf("║     THANKS FOR PLAYING!               ║\n");
    printf("║                                       ║\n");
    printf("║     Hen gap ban lan sau! 👋           ║\n");
    printf("║     See you next time!                ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    printf("\n");
}

int main(void) {
    // KHỞI TẠO HỆ THỐNG AUDIO
    initAudio();

    // GAME LOOP
    while (1) {
        // Hiển thị màn hình menu
        displayMainMenu();
        
        // Chờ người chơi bấm phím bất kỳ
        getchar();

        // PHÁT NHẠC NỀN VÀ BẮT ĐẦU GAME
        playBackgroundMusic();
        startGame();
        stopBackgroundMusic();

        // HỎI CHƠI LẠI
        if (!askPlayAgain()) {
            break;  // Thoát vòng lặp chính
        }
    }

    // HIỂN THỊ MÀN HÌNH KẾT THÚC
    displayEndScreen();

    // DỌN DẸP HỆ THỐNG AUDIO
    cleanupAudio();

    return EXIT_SUCCESS;
}