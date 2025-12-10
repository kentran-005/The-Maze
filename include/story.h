#ifndef STORY_H
#define STORY_H

// check game progress

typedef struct {
    int easyCompleted;
    int normalCompleted;
    int hardCompleted;
} GameProrogress;

// load/save progress

void loadProgress(GameProrogress *progress);
void saveProgress(GameProrogress *progress);

// show story after completing the game

void showStoryEasy (void);
void showStoryNormal (void);
void showStoryHard (void);

// check/show secret ending

int checkSecretEnding(GameProrogress *propress);
void showSecretEnding (void);

// Menu progress

void showProgressMenu(GameProrogress *progress);

#endif