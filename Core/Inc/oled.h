/*
 * oled.h
 *
 *  Created on: 25.10.2026
 *      Author: joelw
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "fonts.h"

typedef struct{
	uint16_t ms;
	uint16_t s;
	uint16_t m;
	uint16_t h;
}zeit_t;


typedef struct{
	uint8_t x;
	uint8_t y;
}coordinates_t;

typedef struct{
	char *text;
	coordinates_t pos;
	FontDef_t *Font;

}oled_line_t;

typedef struct{
	oled_line_t *line;
	uint32_t delay;
}oled_screen_t;

void ShowTime(zeit_t time);
uint8_t ShowText(oled_screen_t *liste, uint8_t list_size, uint8_t *reset);
uint8_t countdown(uint8_t number, uint8_t *reset);
uint8_t show_image(uint8_t reps, uint8_t *reset);

extern const unsigned char  image_1[];
extern const unsigned char  image_2[];

extern oled_line_t screen_1[];
extern oled_line_t screen_2[];
extern oled_line_t screen_3[];
extern oled_line_t screen_4[];

extern oled_screen_t birthday_screens[];


#endif /* INC_OLED_H_ */
