#include "Potentiometer.h" 

void Thermistor_Init(void){
// write this
	SYSCTL_RCGCADC_R |= 0x0001; // Activates ADC0
	SYSCTL_RCGCGPIO_R |= 0x10; // Activate Clock for Port E
	
	while((SYSCTL_PRGPIO_R&0x10) != 0x10){};
		
  GPIO_PORTE_DIR_R &= ~0x08;      // 2) make PE3 input
  GPIO_PORTE_AFSEL_R |= 0x08;     // 3) enable alternate function on PE3
  GPIO_PORTE_DEN_R &= ~0x08;      // 4) disable digital I/O on PE3
  GPIO_PORTE_AMSEL_R |= 0x08;     // 5) enable analog functionality on PE3

	while((SYSCTL_PRADC_R&0x0001) != 0x0001){};    // good code, but not yet implemented in simulator


  ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    maximum speed is 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 8) Sequencer 2 is next priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;					// 10) Enables Software Triggered Interrupts
  ADC0_SSMUX2_R &= ~0x000F;       // 11) clear SS2 field
  ADC0_SSMUX2_R += 0;             //    set channel, maps to AIN0 which samples from PE3
  ADC0_SSCTL2_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0004;           // 13) disable SS2 interrupts
  ADC0_ACTSS_R |= 0x0048;         // 14) enable sample sequencer 2
	Timer5A_Init(&Potentiometer_ISR, 80000, 0); // Timer used to sample the ADC value of the thermistor, samples once every 2 ms
}

void ChangeTime_Mode(uint16_t mode) { // 1 is change time, 0 is none
	// Purpose is to turn the timer on and off of measuring the potentiometer
	
	// MAKE SURE THIS IS OK 
	if (mode) {
		TIMER5_CTL_R = 0x01; // Enable Timer
	} else {
		TIMER5_CTL_R = 0x00000000;  // Disable Timer
	}
}



void Potentiometer_ISR(void) { // Called every 1 ms 
	// ISR needs to read values from Potentiometer_ADC_In
	// Then pass these values to LCD to change time 
}

int32_t Potentiometer_ADC_In(void) {
	int32_t result; 
	ADC0_PSSI_R = 0x0004;            // Initiate SS2
  while((ADC0_RIS_R & 0x04) == 0); // Wait for conversion to complete
	result = ADC0_SSFIFO2_R & 0xFFF; // Read the result (12-bit value)
	ADC0_ISC_R = 0x0004;             // Acknowledge completion
	return result; 
}