#include <stdio.h>
#include "audio.h"

int main() {
    printf("Starting game...\n");
    
    playBackgroundMusic();
    // Bắt đầu game ở đây
    printf("Game running...\n");
    getchar(); // Chờ người dùng nhấn Enter
    
    stopBackgroundMusic();
    
    printf("Game ended.\n");
    return 0;
}