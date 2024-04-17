/*
 * music.h
 *
 *  Created on: Mar 25, 2024
 *      Author: schmidtlw
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#define A 220
#define A_SHARP 233
#define B 247
#define C 261
#define C_SHARP 277
#define D 294
#define D_SHARP 311
#define E 330 //highest note
#define F 175 //lowest note
#define F_SHARP 185
#define G 196
#define G_SHARP 208
#define REST 0

#define quarter_pos 0
#define half_pos 1
#define whole_pos 2
#define eighth_pos 3
#define sixteenth_pos 4

#include <stdint.h>

static uint32_t BPM = 120;



typedef struct note_t {
    uint16_t frequency;
    uint32_t duration;
} note_t;

typedef struct note_length_t {
    uint32_t whole;
    uint32_t half;
    uint32_t quarter;
    uint32_t eighth;
    uint32_t sixteenth;
} note_length_t;




void start_song(note_t* current_song, note_length_t * delays, uint16_t BPM);


#endif /* MUSIC_H_ */
