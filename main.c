#include <stdio.h>
#include "game.h"
#include "audio.h"

int main() {
    char choice;
    
    do {
        printf("Starting game...\n");
        playBackgroundMusic();
        
        startGame();
        
        stopBackgroundMusic();
        
        printf("\nChoi lai? (y/n): ");
        scanf(" %c", &choice);
        getchar(); // Clear buffer
        
    } while (choice == 'y' || choice == 'Y');
    
    printf("\nCam on da choi!\n");
    return 0;
}