#include <stdio.h>
#include "../inc/ST7735.h"
#include "Image.h"
#include "Systick.h"
#include <math.h>

#define CLOCK_CORNER_X 50
#define CLOCK_CORNER_Y 100
#define CLOCK_CENTER_X (CLOCK_CORNER_X + 40)
#define CLOCK_CENTER_Y (CLOCK_CORNER_Y - 40)
#define PI_OVER_180 0.01745

char secondDigits[3];
char minuteDigits[3];
char hourDigits[3];

int minuteHandX, minuteHandY;
int hourHandX, hourHandY;

char digitalTime[9];

void displayNewTime(void);
void convertClockDigit(uint32_t seconds, uint32_t minutes, uint32_t hours);
void generateTime(void);
void appendTime(char *time, int index);
void drawClockHands(void);

void initLCD(void) {
	ST7735_InitR(INITR_REDTAB);

	// init digital clock
	displayNewTime();
	
	// init analog clock
	ST7735_DrawBitmap(CLOCK_CORNER_X, CLOCK_CORNER_Y, clock, CLOCK_WIDTH, CLOCK_HEIGHT);
	drawClockHands();
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

void drawClockHands(void) {
	int minutesAngle = (minutes * 6) - 90;
	minuteHandX = CLOCK_CENTER_X + 22 * cos(minutesAngle * PI_OVER_180);
	minuteHandY = CLOCK_CENTER_Y + 22 * sin(minutesAngle * PI_OVER_180);
	
	int hoursAngle = (hours * 30) - 90;
	hoursAngle += minutesAngle / 12;
	hourHandX = CLOCK_CENTER_X + 12 * cos(hoursAngle * PI_OVER_180);
	hourHandY = CLOCK_CENTER_Y + 12 * sin(hoursAngle * PI_OVER_180);
	
	ST7735_DrawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, minuteHandX, minuteHandY, ST7735_YELLOW);
	ST7735_DrawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, hourHandX, hourHandY, ST7735_YELLOW);
}
