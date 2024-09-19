#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer4A.h"

int32_t Thermistor_ADC_In(void);
void Thermistor_ISR(void);
void Thermistor_Init(void);