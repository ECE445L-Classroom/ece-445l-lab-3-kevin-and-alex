#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#define DELAY10US 160

void Switch_Init(void);
uint32_t SwitchPC4_In(void);
uint32_t SwitchPC5_In(void);
uint32_t SwitchPC6_In(void);
uint32_t SwitchPC7_In(void);
uint32_t SwitchPF0_In(void);
uint32_t SwitchPF4_In(void);
void Clock_Delay(uint32_t ulCount);
void Clock_Delay1ms(uint32_t n);
uint32_t SwitchPC4_Debounce(void);
uint32_t SwitchPC5_Debounce(void);
uint32_t SwitchPC6_Debounce(void);
uint32_t SwitchPC7_Debounce(void);
uint32_t SwitchPF0_Debounce(void);
uint32_t SwitchPF4_Debounce(void);
