// Switch.C
// Interfaces all switches used on lab3

#include "switch.h"

void Switch_Init(void) {
	// Initialization for Port C
	SYSCTL_RCGCGPIO_R |= 0x04; // Activate Clock for Port C
	while ((SYSCTL_PRGPIO_R&0x04) == 0) {}; // Wait for clock to be active
	GPIO_PORTC_DIR_R &= ~0xF0; // Sets pins 4,5,6,7 of Port C as input
	GPIO_PORTC_DEN_R |= 0xF0; // Sets pins 4,5,6,7 of Port C as Digital I/O
	GPIO_PORTF_AFSEL_R &= ~0xF0 ; // Diables Alt function on 4,5,6,7 on Port C
		
	// Intialize Edge-Triggered Interrupts for Port C
	GPIO_PORTC_IS_R &= ~0xF0;  // Make PC4-7 edge-sensitive
	GPIO_PORTC_IBE_R &= ~0xF0; // Trigger on one edge only
	GPIO_PORTC_IEV_R |= 0xF0;  // Trigger on rising edge (button release)
	GPIO_PORTC_ICR_R |= 0xF0;  // Clear any prior interrupts
	GPIO_PORTC_IM_R |= 0xF0;   // Arm interrupt on PC4-7
		
	NVIC_EN0_R |= (1 << 2);    // Enable IRQ 2 for Port C
		
	// Initialization for Port F
	SYSCTL_RCGCGPIO_R |= 0x20; // Activate Clock for Port F
	while ((SYSCTL_PRGPIO_R&0x02) == 0) {}; // Wait for clock to be active
	GPIO_PORTC_DIR_R &= ~0x11; // Sets pins 0, 4 of Port F as input
	GPIO_PORTC_DEN_R |= 0x11; // Sets pins 0, 4 of Port F as Digital I/O
	GPIO_PORTF_AFSEL_R &= ~0x11 ; // Diables Alt function on 0, 4 on Port F
		
	// Intialize Edge-Triggered Interrupts for Port F
	GPIO_PORTF_IS_R &= ~0x11;  // Make PF0, 4 edge-sensitive
	GPIO_PORTF_IBE_R &= ~0x11; // Trigger on one edge only
	GPIO_PORTF_IEV_R |= 0x11;  // Trigger on rising edge (button release)
	GPIO_PORTF_ICR_R |= 0x11;  // Clear any prior interrupts
	GPIO_PORTF_IM_R |= 0x11;   // Arm interrupt on PF0, 4
		
	NVIC_EN0_R |= (1 << 30);   // Enable IRQ 30 for Port F
	
}

void GPIOPortC_Handler(void) {
    if (GPIO_PORTC_RIS_R & 0x10) {  // PC4 caused the interrupt
        GPIO_PORTC_ICR_R = 0x10;    // Acknowledge the interrupt
        
        // Perform debouncing for PC4
        uint32_t Value = SwitchPC4_Debounce();
        if (Value == 0x10) {  // If debounced value is still valid
            // Handler for PC4 here
        }
    }
    if (GPIO_PORTC_RIS_R & 0x20) {  // PC5 caused the interrupt
        GPIO_PORTC_ICR_R = 0x20;    // Acknowledge the interrupt
        
        // Perform debouncing for PC5
        uint32_t Value = SwitchPC5_Debounce();
        if (Value == 0x20) {  // If debounced value is still valid
            // Handler
        }
    }
    if (GPIO_PORTC_RIS_R & 0x40) {  // PC5 caused the interrupt
        GPIO_PORTC_ICR_R = 0x40;    // Acknowledge the interrupt
        
        // Perform debouncing for PC5
        uint32_t Value = SwitchPC6_Debounce();
        if (Value == 0x40) {  // If debounced value is still valid
            /sdsdsdsd
        }
    }
		if (GPIO_PORTC_RIS_R & 0x80) {  // PC5 caused the interrupt
        GPIO_PORTC_ICR_R = 0x80;    // Acknowledge the interrupt
        
        // Perform debouncing for PC5
        uint32_t Value = SwitchPC7_Debounce();
        if (Value == 0x80) {  // If debounced value is still valid
            //sdsdsds
        }
    }
}

void GPIOPortF_Handler(void) {
    if (GPIO_PORTF_RIS_R & 0x01) {  // PF0 caused the interrupt
        GPIO_PORTF_ICR_R = 0x01;    // Acknowledge the interrupt
        
        // Perform debouncing for PF0
        uint32_t Value = SwitchPF0_Debounce();
        if (Value == 0x01) {  // If debounced value is still valid
            // Handle the button release event for PF0
        }
    }
    if (GPIO_PORTF_RIS_R & 0x10) {  // PF4 caused the interrupt
        GPIO_PORTF_ICR_R = 0x10;    // Acknowledge the interrupt
        
        // Perform debouncing for PF4
        uint32_t Value = SwitchPF4_Debounce();
        if (Value == 0x10) {  // If debounced value is still valid
            // Handle the button release event for PF4
        }
    }
}



// Functions for Switch_In() that reads inputs of all switch ports
uint32_t SwitchPC4_In(void) {
	return GPIO_PORTC_DATA_R & 0x10; // 0x10 if pressed
}

uint32_t SwitchPC5_In(void) {
	return GPIO_PORTC_DATA_R & 0x20; // 0x20 if pressed
}

uint32_t SwitchPC6_In(void) {
	return GPIO_PORTC_DATA_R & 0x40; // 0x40 if pressed
}

uint32_t SwitchPC7_In(void) {
	return GPIO_PORTC_DATA_R & 0x80; // 0x80 if pressed
}

uint32_t SwitchPF0_In(void) {
	return GPIO_PORTF_DATA_R & 0x10; // 0x10 if pressed
}

uint32_t SwitchPF4_In(void) {
	return GPIO_PORTF_DATA_R & 0x10; // 0x40 if pressed
}



void Clock_Delay(uint32_t ulCount) {
	while(ulCount) {
		ulCount--;
	}
}


// ------------Clock_Delay1ms------------
// Simple delay function which delays about n milliseconds.
// Inputs: n, number of msec to wait
// Outputs: none
void Clock_Delay1ms(uint32_t n){
  while(n){
    Clock_Delay(23746);  // 1 msec, tuned at 80 MHz, originally part of LCD module
    n--;
  }
}

//------------Switch_Debounce------------ for PC4, 5, 6, 7 and PF0, 4
// Read and return the status of the switch 
// Input: none
// Output: 0x02 if PB1 is high
//         0x00 if PB1 is low
// Passed in value is the switch that is being read
// debounces switch
uint32_t SwitchPC4_Debounce(void){
uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = SwitchPC4_In();
  while(time){
    Clock_Delay(DELAY10US); // 10us
    in = SwitchPC4_In();
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}

uint32_t SwitchPC5_Debounce(void){
uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = SwitchPC5_In();
  while(time){
    Clock_Delay(DELAY10US); // 10us
    in = SwitchPC5_In();
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}

uint32_t SwitchPC6_Debounce(void){
uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = SwitchPC6_In();
  while(time){
    Clock_Delay(DELAY10US); // 10us
    in = SwitchPC6_In();
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}

uint32_t SwitchPC7_Debounce(void){
uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = SwitchPC7_In();
  while(time){
    Clock_Delay(DELAY10US); // 10us
    in = SwitchPC7_In();
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}

uint32_t SwitchPF0_Debounce(void){
uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = SwitchPF0_In();
  while(time){
    Clock_Delay(DELAY10US); // 10us
    in = SwitchPF0_In();
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}

uint32_t SwitchPF4_Debounce(void){
uint32_t in,old,time; 
  time = 1000; // 10 ms
  old = SwitchPF4_In();
  while(time){
    Clock_Delay(DELAY10US); // 10us
    in = SwitchPF4_In();
    if(in == old){
      time--; // same value 
    }else{
      time = 1000;  // different
      old = in;
    }
  }
  return old;
}



