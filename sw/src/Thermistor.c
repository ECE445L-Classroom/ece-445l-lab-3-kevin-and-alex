#include "Thermistor.h"


void Thermistor_Init(void){
// write this
	SYSCTL_RCGCADC_R |= 0x0001; // Activates ADC0
	SYSCTL_RCGCGPIO_R |= 0x10; // Activate Clock for Port E
	
	while((SYSCTL_PRGPIO_R&0x10) != 0x10){};
		
  GPIO_PORTE_DIR_R &= ~0x10;      // 2) make PE4 input and PE3 for potentiometer
  GPIO_PORTE_AFSEL_R |= 0x10;     // 3) enable alternate function on PE4
  GPIO_PORTE_DEN_R &= ~0x10;      // 4) disable digital I/O on PE4
  GPIO_PORTE_AMSEL_R |= 0x10;     // 5) enable analog functionality on PE4

	while((SYSCTL_PRADC_R&0x0001) != 0x0001){};    // good code, but not yet implemented in simulator


  ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    maximum speed is 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;					// 10) Enables Software Triggered Interrupts
  ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
  ADC0_SSMUX3_R += 9;             //    set channel
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
	Timer4A_Init(&Thermistor_ISR, 160000, 0); // Timer used to sample the ADC value of the thermistor, samples once every 2 ms
}

#define BUFSIZE 500 // Buffer is 1000 values for accurate sampling
uint32_t DataBuf[BUFSIZE]; // Array containing the values measured by ADC
volatile uint32_t Num; //Index into the BUF

uint32_t average(void) {
	uint32_t total = 0; 
	for (int i = 0; i < BUFSIZE; i ++) {
		total += DataBuf[i];
	}
	return total / BUFSIZE;
}

void Thermistor_ISR(void) {
	uint32_t ADCvalue; 
	ADCvalue = Thermistor_ADC_In();
  if(Num < BUFSIZE){
    DataBuf[Num] = ADCvalue;
    Num++;
  }
	if (Num == BUFSIZE) {
		uint32_t output = average(); // Output is the signal value of 500 measurements within the second
		Num = 0; 
		// Gotta do Computations here to convert Output to Farenheit Value
		// Call function passing output in to main to display onto LCD
		// UpdateTemperature(output); 
	}
}

int32_t Thermistor_ADC_In(void){
	uint32_t result; 
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
    // if you have an A0-A3 revision number, you need to add an 8 usec wait here
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}

