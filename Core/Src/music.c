/*
 * LED ausgabe Herz mit 16 LEDS gesteuert über 4 Transistoren (NPN)
 *
 * Autor: Joel Woelfel
 */

#include "main.h"
#include "music.h"
#include "led.h"

#define BIRTHDAY_SONG_SIZE 25
#define PAUSE 55
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784


extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

note_t happyBirthday[] =
{
    // Happy Birthday to you
    {G4, 300, 0b0001},
    {G4, 150, 0b0001},
    {A4, 450, 0b0010},
    {G4, 450, 0b0001},
    {C5, 450, 0b0100},
    {B4, 900, 0b1111},

    // Happy Birthday to you
    {G4, 300, 0b0001},
    {G4, 150, 0b0001},
    {A4, 450, 0b0010},
    {G4, 450, 0b0001},
    {D5, 450, 0b1000},
    {C5, 900, 0b1111},

    // Happy Birthday dear ...
    {G4, 300, 0b0001},
    {G4, 150, 0b0001},
    {G5, 450, 0b1000},
    {E5, 450, 0b0100},
    {C5, 450, 0b0010},
    {B4, 450, 0b0001},
    {A4, 900, 0b1111},

    // Happy Birthday to you
    {F5, 300, 0b1000},
    {F5, 150, 0b1000},
    {E5, 450, 0b0100},
    {C5, 450, 0b0010},
    {D5, 450, 0b0110},
    {C5, 900, 0b1111}
};

void buzzer_set_frequency(uint16_t freq)
{

    if(freq == 0)
    {
        __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,0);
        return;
    }


    uint32_t arr = 1000000 / freq - 1;


    __HAL_TIM_SET_AUTORELOAD(&htim4,arr);


    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 50);
}


uint8_t play_happy_birthday(uint8_t reps, uint8_t *reset)
{
	static uint32_t note_timer=0;
	static uint8_t count=0, j=0;

	if(*reset)
	{
		count=0;
		*reset=0;
	}

    if(HAL_GetTick()>=note_timer)
    {
        if(j>=BIRTHDAY_SONG_SIZE)
    	{
    		j=0;
    		count++;
    	}
        
    	if(reps<=count)
    	{
    		return 1;
    	}

    	buzzer_set_frequency(happyBirthday[j].frequency);
    	song_led(happyBirthday[j].led);
    	note_timer=HAL_GetTick()+happyBirthday[j].duration+PAUSE;

    	j++;
    }

    if(HAL_GetTick()>=note_timer-PAUSE)
    	buzzer_set_frequency(0);

    return 0;
}
