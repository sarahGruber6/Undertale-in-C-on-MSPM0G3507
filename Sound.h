// Sound.h
// Runs on MSPM0
// Play sounds on 5-bit DAC.
// Your name
// 11/5/2023
#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>

typedef struct {
    uint16_t frequency;
    uint32_t duration;
} Note;

typedef struct {
    const Note* notes; // pointer
    uint32_t length;   // # of notes
} Song;

// Function Prototypes
void DAC5_Init(void); // Initialize the DAC
void StartSong(uint32_t songIndex); // Start playing a song
void StartNote(Note note); // Start playing a single note
void NoteDurationTimerHandler();
void WaveTableTimerHandler();

// Global Variables
extern const Song songs[]; // Array of songs
extern uint32_t currentNoteIndex; // Index of the current song
extern uint32_t currentSongIndex; // Index of the current song
extern int songFlag;

#endif
