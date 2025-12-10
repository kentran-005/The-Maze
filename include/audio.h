#ifndef AUDIO_H
#define AUDIO_H

// Enable audio system
#define AUDIO_DISABLED 1

void initAudio(void);
void cleanupAudio(void);
void playBackgroundMusic(void); //nhac nen
void stopBackgroundMusic(void);

void playLossMusic(void); //nhac thua cuoc
void playVictoryMusic(void); //nhac chien thang
void playSuka(void); //nhac khi quai vat bi nguoi choi diet
void playPlayerKilled(void); //nhac khi nguoi choi bi quai vat diet
void playMenuSound(void); //nhac menu
void playDrowningSound(void); //nhac khi het oxy
void playInGame(void); //nhac khi dang choi


#endif
