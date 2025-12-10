#ifndef STORY_H
#define STORY_H

// check game progress

typedef struct {
    int easyCompleted;
    int normalCompleted;
    int hardCompleted;
} GameProgress;

// load/save progress

void loadProgress(GameProgress *progress);
void saveProgress(GameProgress *progress);

// show story after completing the game

void showStoryEasy (void);
void showStoryNormal (void);
void showStoryHard (void);
void typeText(const char *text, int delayMs);

// check/show secret ending

int checkSecretEnding(GameProgress *propress);
void showSecretEnding (void);

// Menu progress

static void showProgressMenu(GameProgress *progress);

#endif