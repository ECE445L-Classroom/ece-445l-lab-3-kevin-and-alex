#include <stdint.h>
#include "../inc/Timer0A.h"
#include "Systick.h"
#include "LCD.h"

uint32_t seconds = 0;
uint32_t minutes = 0;
uint32_t hours = 12;

void incrementTime(void);

void initSystick(void) {
	Timer0A_Init(&incrementTime, 80000000, 1);
}

void incrementTime(void) {
	seconds++;
	if (seconds >= 60) {
		minutes++;
		seconds = 0;
	}
	if (minutes >= 60) {
		hours++;
		minutes = 0;
	}
	if (hours >= 24) {
		hours = 0;
	}
	displayNewTime();
}