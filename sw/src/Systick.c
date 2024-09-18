#include <stdint.h>
#include "../inc/Timer0A.h"
#include "Systick.h"
#include "LCD.h"

uint32_t timeInSeconds = 86390;

void incrementTime(void);

void initSystick(void) {
	Timer0A_Init(&incrementTime, 80000000, 1);
}

void incrementTime(void) {
	timeInSeconds++;
	timeInSeconds %= 86400;
	updateDisplay(timeInSeconds);
}
