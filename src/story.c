#include <stdio.h>
#include <unistd.h>
#include "story.h"

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
