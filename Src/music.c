/*
 * music.c
 *
 *  Created on: Mar 25, 2024
 *      Author: schmidtlw
 */

#include "music.h"
#include "piezo.h"
#include "systick.h"


void init_delays(uint16_t BPM, note_length_t* delays) {
    // make BPM into a note lengths
	uint32_t quart = 60000/BPM;
	//printf("quarter in func: %d\n", quart);
    delays->quarter = 60000 / BPM;

    delays->half = delays->quarter * 2;
    delays->whole = delays->quarter * 4;
    delays->eighth = delays->quarter / 2;
    delays->sixteenth = delays->quarter / 4;
}

void start_song(note_t* current_song, note_length_t * delays, uint16_t BPM) {
    init_delays(BPM, delays);
    
    // Determine the delay position for the current note
    int delay_pos = current_song[0].duration;
    
    // Get the delay value from the delays structure based on the position
    uint16_t delay_value;
    switch (delay_pos) {
        case quarter_pos:
            delay_value = delays->quarter;
            break;
        case half_pos:
            delay_value = delays->half;
            break;
        case whole_pos:
            delay_value = delays->whole;
            break;
        case eighth_pos:    
            delay_value = delays->eighth;
            break;
        case sixteenth_pos:
            delay_value = delays->sixteenth;
            break;
        default:
            delay_value = 0;
            return;
    }
    
    // Play the next note with the determined delay value
    //playTone(current_song[0].frequency, delay_value);
}
