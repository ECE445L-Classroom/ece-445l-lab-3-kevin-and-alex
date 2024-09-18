#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer1A.h"
#include "../inc/Timer2A.h"

#define NUM_SAMPLES 32
#define BEEP_TIME 500
#define SILENCE_TIME 500

void PWM0A_Init(uint16_t period, uint16_t duty);

extern const uint16_t sineWave[32];


