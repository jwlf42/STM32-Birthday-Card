/*
 * LED ausgabe Herz mit 16 LEDS gesteuert über 4 Transistoren (NPN)
 *
 * Autor: Joel Woelfel
 */

#include "main.h"
#include "ssd1306.h"
#include "fonts.h"
#include "music.h"
#include "led.h"
#include <stdio.h>

#define HEARTBEAT_SIZE (sizeof(HEARTBEAT_LUT))
#define TEXT_LIST_SIZE 12

extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

uint8_t i = 0, j=0;


const uint8_t HEARTBEAT_LUT[] = {

    // erster gr0ßer Herzschlag
    13, 14, 18, 18, 25, 25, 32, 32, 45, 45, 59, 59, 70, 70, 89, 89, 95, 100, 100, 100,
	100, 95, 95, 95, 89, 89, 85, 85, 85, 79, 79, 75, 65, 65, 50, 50, 45, 35, 25, 20 ,20,

    // kleiner zweiter Herzschlag
    25, 25, 35, 45, 45, 58, 58, 64, 64, 64, 58, 58, 55,
    55, 55, 54, 53, 53, 53, 48, 48, 48, 45, 45, 42, 42, 42, 38, 38, 31, 31,

    // langsames Ausklingen / Ruhephase
    31, 31, 31, 29, 29, 29, 29, 29, 25, 25, 25,
    25, 20, 20, 20, 20, 18, 18, 18, 17, 17, 17, 16,
    16, 16 ,15, 15, 15, 14, 14 ,14,
	13, 13, 13, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12
};

const uint16_t GAMMA_TABLE[101] =
{
		00,
		00,00,00,01,01,02,03,04,05,06,
		8,9,11,13,15,18,20,23,26,29,
		32,36,39,43,47,52,56,61,66,71,
		76,82,87,93,99,106,112,119,126,133,
		141,148,156,164,173,181,190,198,208,218,
		227,237,247,258,268,279,290,302,313,325,
		337,349,362,375,388,401,414,428,442,456,
		471,485,500,516,531,547,563,579,595,612,
		629,646,664,682,699,718,736,755,774,793,
		813,823,852,873,893,914,935,957,978,1000
};
/******************************************************
 * Herz puls animation
 *
*******************************************************/
uint8_t heart_pulse()
{
	static uint32_t heart_timer=0;
	static uint8_t count=0, i=0;
	uint8_t brightness= HEARTBEAT_LUT[i];
	uint16_t pwm;

	if(HAL_GetTick()>=heart_timer)
	{
		heart_timer=HAL_GetTick()+10;

		pwm = GAMMA_TABLE[brightness];

		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwm);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pwm);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pwm);

		i++;

		if(i>=HEARTBEAT_SIZE)
		{
			i=0;
			count++;
		}
	}

	return count;

}


/******************************************************
 * LED Lauflicht einzenle LED gruppen an/ aus.
 * Argumente anzahl widerholung, funktion resetten
 * rückgabewert funktion aktiv/ inaktiv =0
*******************************************************/
uint8_t running_light(uint8_t reps, uint8_t*reset)
{
	static uint32_t light_timer=0;
	static uint8_t light_position=0, count=0;

	if(*reset)
	    {
	    	count=0;
	    	light_timer=0;
	    	*reset=0;
	   	}
	if(reps==count)
		return 1;

    if(HAL_GetTick() >= light_timer)
    {
        light_timer = HAL_GetTick() + 80;

        // alle Gruppen aus
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);

        // aktuelle Gruppe an
        switch(light_position)
        {
            case 0:
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1000);
                break;

            case 1:
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 1000);
                break;

            case 2:
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 1000);
                break;

            case 3:
                __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 1000);
                break;
        }

        light_position++;

        if(light_position >= 4)
        {
        	light_position = 0;
        	count++;
        }
    }
    return 0;
}

/******************************************************
 * LED Lauflicht Band LED gruppen an/ aus.
 * Argumente anzahl widerholung, funktion resetten
 * rückgabewert funktion aktiv/ inaktiv =0
*******************************************************/
uint8_t running_light_build(uint8_t reps, uint8_t *reset)
{
	static uint32_t light_timer=0;
    static int8_t light_position = 0, direction = 1;
    static uint8_t count=0;

    if(*reset)
    {
    	count=0;
    	*reset=0;
   	}

    if(reps==count)
    	return 1;

    if(HAL_GetTick() >= light_timer)
    {
        light_timer = HAL_GetTick() +300;

        // alle aus
        led_off();

        // alle bis zur aktuellen Position an
        for(uint8_t i = 0; i <= light_position; i++)
        {
            switch(i)
            {
                case 0:
                    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1000);
                    break;

                case 1:
                    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 1000);
                    break;

                case 2:
                    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 1000);
                    break;

                case 3:
                    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 1000);
                    break;
            }
        }

        light_position += direction;

        // ganz oben angekommen → wieder abbauen
        if(light_position >= 3)
        {
            direction = -1;
        }

        // ganz unten angekommen → wieder aufbauen
        if(light_position == -1)
        {
            direction = 1;
            count++;
        }
    }
    return 0;
}

/******************************************************
 * schaltet led nach bitmuster an

*******************************************************/
void song_led(uint8_t leds)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (leds & 0b0001) ? 1000 : 0);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (leds & 0b0010) ? 1000 : 0);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (leds & 0b0100) ? 1000 : 0);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, (leds & 0b1000) ? 1000 : 0);
}

void led_off()
{
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}
