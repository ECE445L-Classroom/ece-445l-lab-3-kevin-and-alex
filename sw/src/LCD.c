#include "../inc/ST7735.h"
#include "Image.h"

void initLCD(void) {
	ST7735_InitR(INITR_REDTAB);

	// init clock
	ST7735_DrawBitmap(50, 100, clock, CLOCK_WIDTH, CLOCK_HEIGHT);
}
