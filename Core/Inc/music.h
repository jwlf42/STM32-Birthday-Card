#ifndef MUSIC_H
#define MUSIC_H



typedef struct {
    uint16_t frequency;
    uint16_t duration;
    uint8_t led;
} note_t;


void buzzer_set_frequency(uint16_t freq);
uint8_t play_happy_birthday(uint8_t reps, uint8_t *reset);

#endif
