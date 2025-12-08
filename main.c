#include <stdio.h>
#include "game.h"
#include "audio.h"
#include <stdlib.h>

int main() {
    char choice;

    do {
        // Xóa màn hình trước khi bắt đầu
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif

        printf("Starting game...\n");
        playBackgroundMusic();

        // Bắt đầu game (có menu chọn độ khó bên trong)
        startGame();

        stopBackgroundMusic();

        // ===== PHẦN ĐÃ SỬA: KIỂM TRA Y / N HỢP LỆ =====
        do {
            printf("\n╔═══════════════════════════════════════╗\n");
            printf("║  Ban co muon choi lai khong?         ║\n");
            printf("╚═══════════════════════════════════════╝\n");
            printf("Nhap (y/n): ");
            scanf(" %c", &choice);

            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            if (choice != 'y' && choice != 'Y' &&
                choice != 'n' && choice != 'N') {
                printf("Lua chon khong hop le, vui long chon lai!\n");
            }

        } while (choice != 'y' && choice != 'Y' &&
                 choice != 'n' && choice != 'N');
        // ===== KẾT THÚC PHẦN ĐÃ SỬA =====

    } while (choice == 'y' || choice == 'Y');

    // Kết thúc game
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif

    printf("\n");
    printf("╔═══════════════════════════════════════╗\n");
    printf("║     CAM ON DA CHOI GAME!              ║\n");
    printf("║     THANKS FOR PLAYING!               ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    printf("\n");

    return 0;
}
