#include <stdio.h>
#include "../inc/ST7735.h"
#include "Image.h"
#include "Systick.h"

#define DIGITAL_X 7
#define DIGITAL_Y 9
#define CLOCK_CORNER_X 27
#define CLOCK_CORNER_Y 83
#define CLOCK_CENTER_X (CLOCK_CORNER_X + 40)
#define CLOCK_CENTER_Y (CLOCK_CORNER_Y - 40)
#define PI_OVER_180 0.01745
#define BINFIX 1024
#define SET_TIME_X 16
#define SET_ALARM_X 68
#define SET_ON_OFF_X 110
#define ARROW_Y 135

int sinTable[] = {0, 18, 36, 54, 71, 89, 107, 125, 143, 160, 178, 195, 213, 230, 248, 265, 282, 299, 316, 333, 350, 367, 384, 400, 416, 433, 449, 465, 481, 496, 512, 527, 543, 558, 573, 587, 602, 616, 630, 644, 658, 672, 685, 698, 711, 724, 737, 749, 761, 773, 784, 796, 807, 818, 828, 839, 849, 859, 868, 878, 887, 896, 904, 912, 920, 928, 935, 943, 949, 956, 962, 968, 974, 979, 984, 989, 994, 998, 1002, 1005, 1008, 1011, 1014, 1016, 1018, 1020, 1022, 1023, 1023, 1024, 1024, 1024, 1023, 1023, 1022, 1020, 1018, 1016, 1014, 1011, 1008, 1005, 1002, 998, 994, 989, 984, 979, 974, 968, 962, 956, 949, 943, 935, 928, 920, 912, 904, 896, 887, 878, 868, 859, 849, 839, 828, 818, 807, 796, 784, 773, 761, 749, 737, 724, 711, 698, 685, 672, 658, 644, 630, 616, 602, 587, 573, 558, 543, 527, 512, 496, 481, 465, 449, 433, 416, 400, 384, 367, 350, 333, 316, 299, 282, 265, 248, 230, 213, 195, 178, 160, 143, 125, 107, 89, 71, 54, 36, 18, 0, -18, -36, -54, -71, -89, -107, -125, -143, -160, -178, -195, -213, -230, -248, -265, -282, -299, -316, -333, -350, -367, -384, -400, -416, -433, -449, -465, -481, -496, -512, -527, -543, -558, -573, -587, -602, -616, -630, -644, -658, -672, -685, -698, -711, -724, -737, -749, -761, -773, -784, -796, -807, -818, -828, -839, -849, -859, -868, -878, -887, -896, -904, -912, -920, -928, -935, -943, -949, -956, -962, -968, -974, -979, -984, -989, -994, -998, -1002, -1005, -1008, -1011, -1014, -1016, -1018, -1020, -1022, -1023, -1023, -1024, -1024, -1024, -1023, -1023, -1022, -1020, -1018, -1016, -1014, -1011, -1008, -1005, -1002, -998, -994, -989, -984, -979, -974, -968, -962, -956, -949, -943, -935, -928, -920, -912, -904, -896, -887, -878, -868, -859, -849, -839, -828, -818, -807, -796, -784, -773, -761, -749, -737, -724, -711, -698, -685, -672, -658, -644, -630, -616, -602, -587, -573, -558, -543, -527, -512, -496, -481, -465, -449, -433, -416, -400, -384, -367, -350, -333, -316, -299, -282, -265, -248, -230, -213, -195, -178, -160, -143, -125, -107, -89, -71, -54, -36, -18};
int cosTable[] = {1024, 1024, 1023, 1023, 1022, 1020, 1018, 1016, 1014, 1011, 1008, 1005, 1002, 998, 994, 989, 984, 979, 974, 968, 962, 956, 949, 943, 935, 928, 920, 912, 904, 896, 887, 878, 868, 859, 849, 839, 828, 818, 807, 796, 784, 773, 761, 749, 737, 724, 711, 698, 685, 672, 658, 644, 630, 616, 602, 587, 573, 558, 543, 527, 512, 496, 481, 465, 449, 433, 416, 400, 384, 367, 350, 333, 316, 299, 282, 265, 248, 230, 213, 195, 178, 160, 143, 125, 107, 89, 71, 54, 36, 18, 0, -18, -36, -54, -71, -89, -107, -125, -143, -160, -178, -195, -213, -230, -248, -265, -282, -299, -316, -333, -350, -367, -384, -400, -416, -433, -449, -465, -481, -496, -512, -527, -543, -558, -573, -587, -602, -616, -630, -644, -658, -672, -685, -698, -711, -724, -737, -749, -761, -773, -784, -796, -807, -818, -828, -839, -849, -859, -868, -878, -887, -896, -904, -912, -920, -928, -935, -943, -949, -956, -962, -968, -974, -979, -984, -989, -994, -998, -1002, -1005, -1008, -1011, -1014, -1016, -1018, -1020, -1022, -1023, -1023, -1024, -1024, -1024, -1023, -1023, -1022, -1020, -1018, -1016, -1014, -1011, -1008, -1005, -1002, -998, -994, -989, -984, -979, -974, -968, -962, -956, -949, -943, -935, -928, -920, -912, -904, -896, -887, -878, -868, -859, -849, -839, -828, -818, -807, -796, -784, -773, -761, -749, -737, -724, -711, -698, -685, -672, -658, -644, -630, -616, -602, -587, -573, -558, -543, -527, -512, -496, -481, -465, -449, -433, -416, -400, -384, -367, -350, -333, -316, -299, -282, -265, -248, -230, -213, -195, -178, -160, -143, -125, -107, -89, -71, -54, -36, -18, 0, 18, 36, 54, 71, 89, 107, 125, 143, 160, 178, 195, 213, 230, 248, 265, 282, 299, 316, 333, 350, 367, 384, 400, 416, 433, 449, 465, 481, 496, 512, 527, 543, 558, 573, 587, 602, 616, 630, 644, 658, 672, 685, 698, 711, 724, 737, 749, 761, 773, 784, 796, 807, 818, 828, 839, 849, 859, 868, 878, 887, 896, 904, 912, 920, 928, 935, 943, 949, 956, 962, 968, 974, 979, 984, 989, 994, 998, 1002, 1005, 1008, 1011, 1014, 1016, 1018, 1020, 1022, 1023, 1023, 1024};

uint32_t hours, minutes, seconds;
	
char secondDigits[3];
char minuteDigits[3];
char hourDigits[3];

int minuteHandX, minuteHandY;
int hourHandX, hourHandY;

char digitalTime[9];
	
uint32_t currArrowPos;
uint32_t mainMenuPos[] = {SET_TIME_X, SET_ALARM_X, SET_ON_OFF_X};

void displayNewTime(void);
void convertClockDigit(uint32_t seconds, uint32_t minutes, uint32_t hours);
void generateTime(void);
void appendTime(char *time, int index);
void drawClockHands(void);
void drawArrow(uint32_t x, uint32_t y);

void initLCD(void) {
	ST7735_InitR(INITR_REDTAB);

	// init digital clock
	displayNewTime();
	
	// init analog clock
	ST7735_DrawBitmap(CLOCK_CORNER_X, CLOCK_CORNER_Y, clock, CLOCK_WIDTH, CLOCK_HEIGHT);
	drawClockHands();
	
	// init main menu
	currArrowPos = 0;
	ST7735_SetCursor(1, 14);
	ST7735_OutString("TIME");
	drawArrow(mainMenuPos[currArrowPos], ARROW_Y);
	
	ST7735_SetCursor(9, 14);
	ST7735_OutString("ALARM");
	
	ST7735_SetCursor(17, 14);
	ST7735_OutString("OFF");
}

void updateDisplay(uint32_t timeInSeconds) {
	hours = timeInSeconds / 3600 % 24;
	timeInSeconds %= 3600;
	minutes = timeInSeconds / 60;
	seconds = timeInSeconds % 60;
	displayNewTime();
	drawClockHands();
}

void displayNewTime(void) {
	ST7735_SetCursor(DIGITAL_X, DIGITAL_Y);
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
	if (minutesAngle < 0) {
		minutesAngle += 360;
	}
	int temp_minuteHandX = minuteHandX;
	int temp_minuteHandY = minuteHandY;
	minuteHandX = (22 * cosTable[minutesAngle]) / BINFIX;
	minuteHandX += CLOCK_CENTER_X;
	minuteHandY = (22 * sinTable[minutesAngle]) / BINFIX;
	minuteHandY += CLOCK_CENTER_Y;
	if (temp_minuteHandX != minuteHandX || temp_minuteHandY != minuteHandY) {
		ST7735_DrawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, temp_minuteHandX, temp_minuteHandY, ST7735_BLACK);
	}
	ST7735_DrawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, minuteHandX, minuteHandY, ST7735_WHITE);
	
	int hoursAngle = ((hours % 12) * 30) - 90;
	hoursAngle += ((minutesAngle + 90) % 360) / 12;
	if (hoursAngle < 0) {
		hoursAngle += 360;
	}
	int temp_hourHandX = hourHandX;
	int temp_hourHandY = hourHandY;
	hourHandX = (12 * cosTable[hoursAngle]) / BINFIX;
	hourHandX += CLOCK_CENTER_X;
	hourHandY = (12 * sinTable[hoursAngle]) / BINFIX;
	hourHandY += CLOCK_CENTER_Y;
	if (temp_hourHandX != hourHandX || temp_hourHandY != hourHandY) {
		ST7735_DrawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, temp_hourHandX, temp_hourHandY, ST7735_BLACK);
	}
	ST7735_DrawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, hourHandX, hourHandY, ST7735_LIGHTGREY);
}

void drawArrow(uint32_t x, uint32_t y) {
	// (x, y) will be the tip of the arrow
	ST7735_DrawLine(x, y, x - 3, y - 3, ST7735_YELLOW);
	ST7735_DrawLine(x, y, x + 3, y - 3, ST7735_YELLOW);
}
