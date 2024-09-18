// File for the sound

#include "Speaker.h"

void WaveformUpdate(void);
void BeepControl(void);

const uint16_t sineWave[NUM_SAMPLES] = {
	128, 176, 218, 250, 255, 250, 218, 176, 128, 80, 38, 6, 0, 6, 38, 80,
	128, 176, 218, 250, 255, 250, 218, 176, 128, 80, 38, 6, 0, 6, 38, 80
};

// period is 16-bit number of PWM clock cycles in one period (3<=period)
// period for PB6 and PB7 must be the same
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
// PWM clock rate = processor clock rate/SYSCTL_RCC_PWMDIV
//                = BusClock/2
//                = 80 MHz/2 = 40 MHz (in this example)
// Output on PB1/M0PWM0
void PWM0A_Init(uint16_t period, uint16_t duty){
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  while((SYSCTL_PRGPIO_R&0x02) == 0){};
		
  GPIO_PORTB_AFSEL_R |= 0x02;           // enable alt funct on PB1
  GPIO_PORTB_PCTL_R &= ~0x000000F0;     // configure PB1 as PWM0
  GPIO_PORTB_PCTL_R |= 0x00000040;
  GPIO_PORTB_AMSEL_R &= ~0x02;          // disable analog functionality on PB1
  GPIO_PORTB_DEN_R |= 0x02;             // enable digital I/O on PB1
		
  SYSCTL_RCC_R = 0x00100000 |           // 3) use PWM divider
      (SYSCTL_RCC_R & (~0x000E0000));   //    configure for /2 divider
		
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENA_R = 0x8C;                 // high on LOAD, low on CMPA down
  // PB1 goes high on LOAD
  // PB1 goes low on CMPA down
  PWM0_0_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_0_CMPA_R = duty - 1;             // 6) count value when output rises
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000002;          // enable PB1/M0PWM1
	Timer1A_Init(&WaveformUpdate, 16000, 0);									// Timer1A for updating sine wave (440 hz)
	Timer2A_Init(&BeepControl, 16000000 / 1000, 0); 				// Timer 2A for beep / silence (1 ms)
}

volatile uint8_t BeepOn = 1; // Flag to track the beep

void WaveformUpdate(void) {
	static uint8_t index = 0;
	
	if (BeepOn) {
		PWM0_0_CMPA_R = (sineWave[index] * PWM0_0_LOAD_R) / 255;
		index = (index + 1) % 32; // Cycle through the sine wave
	}
}
	
void BeepControl(void) {
	static uint32_t BeepCounter = 0; 
	BeepCounter++;
	
	if (BeepOn && BeepCounter >= 500) {
		BeepOn = 0; // Turn off Beep
		PWM0_ENABLE_R &= ~0x01; // Diable PWM
		BeepCounter = 0;
	} else if (!BeepOn && BeepCounter >= 500) {
		BeepOn = 1; // Turn on Beep
		PWM0_ENABLE_R |= 0x01; // Turn on PWM
		BeepCounter = 0;
	}
}



