#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer5A.h"

int32_t Potentiometer_ADC_In(void);
void Potentiometer_ISR(void); 
void ChangeTime_Mode(uint16_t mode);
