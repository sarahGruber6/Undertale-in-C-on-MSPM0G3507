// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "DAC5.h"
#include "../inc/Timer.h"
#include "SoundData.h"


uint32_t currentNoteIndex = 0;
uint32_t currentSongIndex = 0; // Index of the current song
int songFlag = 0;
#define silent 0
#define MID 16
#define waveSize 32

const uint8_t wave[32] = {
  16,19,21,24,26,28,30,31,
  31,31,30,28,26,24,21,19,
  16,12,10,7,5,3,1,0,
  0,0,1,3,5,7,10,12
};

const Note megalovaniaStart[] = {
    {293, 140},
    {silent, 140},
    {293, 122},
    {silent, 122},
    {587, 248},
    {silent, 248},
    {440, 368},
    {silent, 368},
    {415, 256},
    {silent, 256},
    {392, 242},
    {silent, 242},
    {349, 251},
    {silent, 251},
    {293, 121},
    {silent, 121},
    {349, 121},
    {silent, 121},
    {392, 128},
    {silent, 128},

    {261, 140},
    {silent, 140},
    {261, 140},
    {silent, 140},
    {587, 248},
    {silent, 248},
    {440, 368},
    {silent, 368},
    {415, 256},
    {silent, 256},
    {392, 242},
    {silent, 242},
    {349, 251},
    {silent, 251},
    {293, 121},
    {silent, 121},
    {349, 121},
    {silent, 121},
    {392, 128},
    {silent, 128},

    {246, 140},
    {silent, 140},
    {246, 140},
    {silent, 140},
    {587, 248},
    {silent, 248},
    {440, 368},
    {silent, 368},
    {415, 256},
    {silent, 256},
    {392, 242},
    {silent, 242},
    {349, 251},
    {silent, 251},
    {293, 121},
    {silent, 121},
    {349, 121},
    {silent, 121},
    {392, 128},
    {silent, 128},

    {233, 140},
    {silent, 140},
    {233, 140},
    {silent, 140},
    {587, 248},
    {silent, 248},
    {440, 368},
    {silent, 368},
    {415, 256},
    {silent, 256},
    {392, 242},
    {silent, 242},
    {349, 251},
    {silent, 251},
    {293, 121},     // d
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {392, 128},     // g
    {silent, 121},
};

const Note megalovania[] = {
    {349, 240},     // f
    {silent, 240},
    {349, 121},     // f
    {silent, 121},
    {349, 121},     // f
    {silent, 240},
    {349, 121},     // f
    {silent, 240},
    {349, 240},     // f
    {silent, 240},
    {293, 240},     // d
    {silent, 240},
    {293, 625},     // d
    {silent, 625},
    {293, 240},     // d
    {silent, 240},
    {349, 240},     // f
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {349, 121},     // f
    {silent, 240},
    {392, 240},     // g
    {silent, 240},
    {415, 240},     // ab
    {silent, 240},
    {392, 121},     // g
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {293, 121},     // d
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {392, 240},     // g
    {silent, 480},
    {349, 240},     // f
    {silent, 240},
    {349, 121},     // f
    {silent, 121},
    {349, 121},     // f
    {silent, 240},
    {392, 240},     // g
    {silent, 240},
    {415, 240},     // ab
    {silent, 240},
    {440, 240},     // a
    {silent, 240},
    {523, 240},     // c
    {silent, 240},
    {440, 380},     // a
    {silent, 480},
    {587, 121},     // d
    {silent, 240},
    {587, 121},     // d
    {silent, 240},
    {587, 121},     // d
    {silent, 121},
    {440, 121},     // a
    {silent, 121},
    {587, 121},     // d
    {silent, 121},
    {523, 400},     // c

    {silent, 1600},

    {440, 240},     // a
    {silent, 240},
    {440, 121},     // a
    {silent, 121},
    {440, 121},     // a
    {silent, 121},
    {440, 240},     // a
    {silent, 240},
    {440, 240},     // a
    {silent, 240},
    {392, 240},     // g
    {silent, 240},
    {392, 380},     // g
    {silent, 900},

    {440, 240},     // a
    {silent, 240},
    {440, 121},     // a
    {silent, 121},
    {440, 121},     // a
    {silent, 240},
    {440, 240},     // a
    {silent, 240},
    {392, 240},     // g
    {silent, 240},
    {440, 240},     // a
    {silent, 240},
    {587, 240},     // d
    {silent, 240},
    {440, 121},     // a
    {silent, 121},
    {392, 240},     // g
    {silent, 240},
    {587, 240},     // d
    {silent, 240},
    {440, 240},     // a
    {silent, 240},
    {392, 240},     // g
    {silent, 240},
    {349, 240},     // f
    {silent, 240},
    {523, 240},     // c
    {silent, 240},
    {440, 240},     // a
    {silent, 240},
    {392, 240},     // g
    {silent, 240},
    {349, 240},     // f
    {silent, 240},
    {294, 240},     // d
    {silent, 240},
    {330, 121},     // e
    {silent, 121},
    {349, 240},     // f
    {silent, 240},
    {440, 240},     // a
    {silent, 240},
    {523, 500},     // c

    {silent, 2250},

    {349, 121},     // f
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {392, 121},     // g
    {silent, 121},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {393, 400},     // g

    {silent, 1000},

    {415, 121},     // ab
    {silent, 240},
    {440, 121},     // a
    {silent, 121},
    {523, 121},     // c
    {silent, 240},
    {440, 121},     // a
    {silent, 121},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {330, 121},     // e
    {silent, 121},
    {349, 121},     // f
    {silent, 240},
    {393, 121},     // g
    {silent, 240},
    {415, 121},     // ab
    {silent, 240},
    {523, 121},     // c
    {silent, 480},
    {554, 121},     // c#
    {silent, 240},
    {415, 121},     // ab
    {silent, 240},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {393, 400},     // g

    {silent, 1100},

    {175, 240},     // f
    {silent, 240},
    {196, 240},     // g
    {silent, 240},
    {220, 240},     // a
    {silent, 240},
    {349, 121},     // f
    {silent, 240},
    {330, 300},     // e
    {silent, 720},
    {294, 300},     // d
    {silent, 720},
    {330, 300},     // e
    {silent, 720},
    {349, 300},     // f
    {silent, 720},
    {393, 300},     // g
    {silent, 720},
    {330, 300},     // e
    {silent, 720},
    {440, 500},     // a

    {silent, 1000},

    {440, 121},     // a
    {silent, 121},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {370, 121},     // f#
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {330, 121},     // e
    {silent, 121},
    {311, 121},     // eb
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {277, 1500},    // c#
    {311, 1500},    // eb

    {silent, 2000},

    {349, 121},     // f
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {392, 121},     // g
    {silent, 121},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {393, 400},     // g

    {silent, 1000},

    {415, 121},     // ab
    {silent, 240},
    {440, 121},     // a
    {silent, 121},
    {523, 121},     // c
    {silent, 240},
    {440, 121},     // a
    {silent, 121},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {330, 121},     // e
    {silent, 121},
    {349, 121},     // f
    {silent, 240},
    {393, 121},     // g
    {silent, 240},
    {415, 121},     // ab
    {silent, 240},
    {523, 121},     // c
    {silent, 480},
    {554, 121},     // c#
    {silent, 240},
    {415, 121},     // ab
    {silent, 240},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {393, 400},     // g

    {silent, 1100},

    {175, 240},     // f
    {silent, 240},
    {196, 240},     // g
    {silent, 240},
    {220, 240},     // a
    {silent, 240},
    {349, 121},     // f
    {silent, 240},
    {330, 300},     // e
    {silent, 720},
    {294, 300},     // d
    {silent, 720},
    {330, 300},     // e
    {silent, 720},
    {349, 300},     // f
    {silent, 720},
    {393, 300},     // g
    {silent, 720},
    {330, 300},     // e
    {silent, 720},
    {440, 500},     // a

    {silent, 1000},

    {440, 121},     // a
    {silent, 121},
    {415, 121},     // ab
    {silent, 121},
    {393, 121},     // g
    {silent, 121},
    {370, 121},     // f#
    {silent, 121},
    {349, 121},     // f
    {silent, 121},
    {330, 121},     // e
    {silent, 121},
    {311, 121},     // eb
    {silent, 121},
    {294, 121},     // d
    {silent, 121},
    {277, 1500},    // c#
    {311, 1500},    // eb

    {silent, 1000},

    {233, 3000},    // bb
    {349, 1000},    // f
    {330, 2000},    // e
    {294, 2000},    // d
    {349, 8000},    // f
    {silent, 400},

    {233, 3000},    // bb
    {349, 1000},    // f
    {330, 2000},    // e
    {294, 2000},    // d
    {silent, 121},
    {294, 3500},    // d
    {silent, 400},

    {233, 3000},    // bb
    {349, 1000},    // f
    {330, 2000},    // e
    {294, 2000},    // d
    {349, 8000},    // f
    {silent, 400},

    {233, 3000},    // bb
    {349, 1000},    // f
    {330, 2000},    // e
    {294, 2000},    // d
    {silent, 121},
    {294, 3500},    // d


};

const Note silenceSong[] = {
    {silent, 100000},   // silence
};

const Note sansTalk[] = {
    {165, 121},
};

const Song songs[] = {
    {megalovaniaStart, sizeof(megalovaniaStart) / sizeof(megalovaniaStart[0])}, // 0
    {megalovania, sizeof(megalovania) / sizeof(megalovania[0])},                // 1
    {silenceSong, sizeof(silenceSong) / sizeof(silenceSong[0])},                // 2
    {sansTalk, sizeof(sansTalk) / sizeof(sansTalk[0])},                         // 3
    // ... more songs
};

void SetUpNoteDurationTimer(uint32_t duration) {
    // frequency = TimerClock/prescale/period

    uint32_t period = 40000 * duration;

    TimerG12_IntArm(period, 7);
}

void SetUpWaveTableTimer(uint32_t stepRate) {
    uint32_t timerTicks = 40000000 / stepRate;

    TimerG8_IntArm(timerTicks, 1, 7); // prescale 1, priority 7
}

void WaveTableTimerHandler() {
    static uint32_t waveTableIndex = 0;
    DAC5_Out(wave[waveTableIndex]);
    waveTableIndex = (waveTableIndex + 1) % waveSize;
}

uint32_t CalculateWaveTableStepRate(uint16_t frequency) {
    return (32 * frequency);            // initially divided 40000000 by step rate
}

void StartNote(Note note) {
        uint32_t waveTableStepRate = CalculateWaveTableStepRate(note.frequency);
        SetUpWaveTableTimer(waveTableStepRate);
    SetUpNoteDurationTimer(note.duration);
}

void StartSong(uint32_t songIndex) {
    if (currentSongIndex != songIndex) {
        currentNoteIndex = 0;  // reset index only if a different song is started
    }
    currentSongIndex = songIndex;
    StartNote(songs[currentSongIndex].notes[currentNoteIndex]);
}

void NoteDurationTimerHandler() {
    if (currentNoteIndex < songs[currentSongIndex].length) {
        StartNote(songs[currentSongIndex].notes[currentNoteIndex]);
    } else {
        songFlag = 1;
        if(currentSongIndex == 0){
            StartSong(1);
        }else if (currentSongIndex == 1){
            StartSong(0);
        }else if (currentSongIndex == 3){
            StartSong(2);
        }
        currentNoteIndex = 0;
        // StartSong(2);    // silent
    }
    currentNoteIndex++;

}

void TIMG12_IRQHandler(void) {
    if((TIMG12->CPU_INT.IIDX) == 1){
        NoteDurationTimerHandler();
    }
}

void TIMG8_IRQHandler(void) {
    if((TIMG8->CPU_INT.IIDX) == 1){
        WaveTableTimerHandler();
    }
}





