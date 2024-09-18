// File for the sound

#include "Speaker.h"

void BeepControl(void);
void ToggleSound(void);

volatile uint8_t BeepOn = 0; // Flag to track the beep
volatile uint8_t Alarm = 0; // Flag that turns on and off alarm

void Speaker_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_PRGPIO_R&0x02) == 0){};
	
	GPIO_PORTB_AFSEL_R &= ~0x02;           // disable alt funct on PB1
  GPIO_PORTB_PCTL_R &= ~0x000000F0;     // configure PB1 as GPIO
  GPIO_PORTB_AMSEL_R &= ~0x02;          // disable analog functionality on PB1
  GPIO_PORTB_DEN_R |= 0x02;             // enable digital I/O on PB1
	GPIO_PORTB_DIR_R |= 0x02; 						// Enable Output
	Timer1A_Init(&ToggleSound, 80000, 0); // 1Ms Timer
	Timer2A_Init(&BeepControl, 40000000, 0); // 500 Ms Timer
}


	
void BeepControl(void) {
	if (!Alarm) { // If Alarm is off
		BeepOn = 0; // Turn off Beep
		GPIO_PORTB_DATA_R &= ~0x02; // Makes sure the sound is off
	} else 
	if (BeepOn) {
		BeepOn = 0; // Turn off Beep
		GPIO_PORTB_DATA_R &= ~0x02; // Turns Sound off
	} else { // Alarm is currently off
		BeepOn = 1; // Turn on Beep
		GPIO_PORTB_DATA_R |= 0x02; 
	}
}

void ToggleSound(void) {
	if (BeepOn) {		
		GPIO_PORTB_DATA_R ^= 0x02; 
	} else {
		GPIO_PORTB_DATA_R &= ~0x02; 
	}
}



