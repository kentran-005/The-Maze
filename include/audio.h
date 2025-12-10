#ifndef AUDIO_H
#define AUDIO_H

// Enable audio system
#define AUDIO_DISABLED 1

void initAudio(void);
void cleanupAudio(void);
void playBackgroundMusic(void);
void stopBackgroundMusic(void);
void playLossMusic(void);
void playVictoryMusic(void);

#endif
