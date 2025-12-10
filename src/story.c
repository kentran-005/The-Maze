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
void showPrologue(void) {
    printf("\n\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║                                               ║\n");
    printf("║              T H E   M A Z E                  ║\n");
    printf("║                                               ║\n");
    printf("║           「 Mở Đầu: Bức Thư Bí Ẩn 」         ║\n");
    printf("║                                               ║\n");
    printf("╚═══════════════════════════════════════════════╝\n\n");
    
    sleep_ms(1500);
    
    typeText("Sa mạc Mesopotamia, năm 2024...\n\n", 30);
    sleep_ms(1000);
    
    typeText("Nhà khảo cổ học Cyr nhận được một bức thư không người gửi.\n", 30);
    sleep_ms(800);
    typeText("Trong đó là tọa độ dẫn tới một công trình cổ đại\n", 30);
    sleep_ms(800);
    typeText("chưa từng được ghi nhận trong lịch sử...\n\n", 30);
    sleep_ms(1200);
    
    printf("+-----------------------------------------------+\n");
    printf("|  \"Nếu ngươi tìm kiếm sự thật về quá khứ,     |\n");
    printf("|   hãy đến nơi mặt trời chạm đất.             |\n");
    printf("|   Dưới lớp cát ngàn năm là lời giải đáp       |\n");
    printf("|   cho những gì ngươi đã quên...\"             |\n");
    printf("+-----------------------------------------------+\n\n");
    
    sleep_ms(1500);
    
    typeText("Sau nhiều ngày tìm kiếm, Cyr đứng trước cánh cửa hầm bí ẩn.\n", 30);
    sleep_ms(800);
    typeText("Trên cửa khắc hình tượng nữ thần biển cả cổ đại - Tiamat.\n", 30);
    sleep_ms(800);
    typeText("Và một dòng chữ bằng chữ hình nêm:\n\n", 30);
    sleep_ms(1000);
    
    printf("     「 Kẻ bước vào đây sẽ gặp lại chính mình 」\n\n");
    sleep_ms(1500);
    
    typeText("Không biết tại sao, tim Cyr đập mạnh...\n", 30);
    sleep_ms(500);
    typeText("Như thể nơi này đang gọi tên anh.\n", 30);
    sleep_ms(500);
    typeText("Như thể... anh từng ở đây rồi.\n\n", 30);
    sleep_ms(1500);
    
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║         Hành trình bắt đầu từ đây...         ║\n");
    printf("╚═══════════════════════════════════════════════╝\n\n");
    
    printf("Nhấn Enter để bước vào mê cung...");
    getchar();
}
void showInterludeAfterEasy(void) {
    printf("\n");
    printf("+===============================================+\n");
    printf("|              Chương 1: Hồi Ức Mờ Nhạt        |\n");
    printf("+===============================================+\n\n");
    
    typeText("Trong giấc mơ mơ hồ, Cyr nghe thấy tiếng sóng...\n", 30);
    sleep_ms(800);
    typeText("Một giọng nói xa xăm vọng lại:\n", 30);
    sleep_ms(500);
    typeText("\"Con... có nhớ ta không?\"\n\n", 30);
    sleep_ms(1200);
    
    printf("Hình ảnh một bóng dáng khổng lồ hiện ra giữa màn đêm.\n");
    printf("Nhưng mọi thứ lại tan biến khi Cyr tỉnh dậy...\n\n");
    
    printf("Nhấn Enter để tiếp tục...");
    getchar();
}

void showInterludeAfterNormal(void) {
    printf("\n");
    printf("+===============================================+\n");
    printf("|           Chương 2: Mảnh Ghép Quá Khứ       |\n");
    printf("+===============================================+\n\n");
    
    typeText("Lần này, giấc mơ rõ ràng hơn...\n", 30);
    sleep_ms(800);
    typeText("Cyr nhìn thấy chính mình đang đứng trước một ngôi đền cổ.\n", 30);
    sleep_ms(500);
    typeText("Trên tay là một viên đá phát sáng màu xanh thẫm.\n\n", 30);
    sleep_ms(1200);
    
    printf("\"Kingu... đứa con của ta...\"\n");
    sleep_ms(800);
    printf("Giọng nói ấy... tại sao lại quen thuộc đến thế?\n\n");
    
    printf("Nhấn Enter để tiếp tục...");
    getchar();
}

void showInterludeAfterHard(void) {
    printf("\n");
    printf("+===============================================+\n");
    printf("|            Chương 3: Mộng Tàn         |\n");
    printf("+===============================================+\n\n");
    
    typeText("Cyr đứng trước gương, nhưng người trong gương không phải là anh.\n", 30);
    sleep_ms(1000);
    typeText("Đó là một người mang dánh hình tương đương cùng mái tóc dài và sợi xích trên tay.\n", 30);
    sleep_ms(800);
    typeText("Và viên đá... đang nằm ngay trong lồng ngực anh ta.\n\n", 30);
    sleep_ms(1500);
    
    printf("\"Con đã sẵn sàng chấp nhận số phận chưa?\"\n");
    sleep_ms(1000);
    printf("\"Hay con sẽ tiếp tục chạy trốn?\"\n\n");
    
    printf("Nhấn Enter để tiếp tục...");
    getchar();
}

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
    printf("|          Trái tim trên tay từ từ tan biến đi và cũng là lúc thuỷ triều đen dần dần phá huỷ toàn bộ công trình mê cung. Sự sống của Cyr đã hoàn toàn biến mất và thay vào đó là Kingu trỗi dậy thống lĩnh toàn bộ quyền năng xa xưa của hắn ,Để lần nữa chinh phạt cùng Tiamat.                     |\n");
    printf("|                                               |\n");
    printf("+===============================================+\n\n");
    
    sleep_ms(2000);
    // playStoryFromFile("story_hard.txt", 30);

    printf("+===============================================+\n");
    printf("|                                               |\n");
    printf("|          CHUC MUNG! BAN DA HOAN THANH.        |\n");
    printf("|                    THE-MAZE                   |\n");
    printf("|                                               |\n");
    printf("|               TRUE ENDING UNLOCKED           |\n");
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