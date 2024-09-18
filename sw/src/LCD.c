#include <stdio.h>
#include "../inc/ST7735.h"
#include "Image.h"
#include "Systick.h"

char secondDigits[3];
char minuteDigits[3];
char hourDigits[3];
char digitalTime[9];

void displayNewTime(void);
void convertClockDigit(uint32_t seconds, uint32_t minutes, uint32_t hours);
void generateTime(void);
void appendTime(char *time, int index);


void initLCD(void) {
	ST7735_InitR(INITR_REDTAB);

	// init clock
	displayNewTime();
	ST7735_DrawBitmap(50, 100, clock, CLOCK_WIDTH, CLOCK_HEIGHT);
}

void displayNewTime(void) {
	ST7735_SetCursor(0,0);
	generateTime();
	ST7735_OutString(digitalTime);
}

void convertClockDigit(uint32_t seconds, uint32_t mintues, uint32_t hours) {
	sprintf(secondDigits, "%02d", seconds);
	sprintf(minuteDigits, "%02d", minutes);
	sprintf(hourDigits, "%02d", hours);
}

void generateTime(void) {
	convertClockDigit(seconds, minutes, hours);
	appendTime(hourDigits, 0);
	digitalTime[2] = ':';
	appendTime(minuteDigits, 3);
	digitalTime[5] = ':';
	appendTime(secondDigits, 6);
	digitalTime[8] = '\0';
}

void appendTime(char *time, int index) {
	for (int i = 0; i < 2; i++) {
		digitalTime[index] = time[i];
		index++;
	}
}
