#include "mbed.h"


uint16_t TimeStamp1;
uint16_t TimeStamp2;
uint16_t TimeStamp3;
uint16_t TimeStamp4;
uint16_t percentage1;
uint16_t percentage2;
    
uint8_t Start1=1;
uint8_t Start2=255;
uint8_t Start3=4;
uint8_t byte1;
uint8_t byte2;
uint8_t byte3;
uint8_t byte4;


void TIM15_IRQHandler(void) {
    
    TIM15->SR &= ~TIM_SR_UIF;											// Clear UIF flag
     
    if (!(TIM15->SR & TIM_SR_CC2OF)&& !(TIM15->SR & TIM_SR_CC1OF)){
        
            if (TIM15->SR & TIM_SR_CC2IF)
            {
                TimeStamp1 = TIM15->CCR2;  								// IC2 capture pulse width measurement
                TIM15->SR = ~TIM_SR_CC2IF; 								// Clear flag on cahnnel CC2
                while (!(TIM15->SR & TIM_SR_CC1IF)); 					// Loop until the capture event flag is set
                TimeStamp2 = TIM15->CCR1; 								// IC1 capture period measurement
                TIM15->SR = ~TIM_SR_CC2IF; 								// Clear flag on cahnnel CC2   
            }  
    }
            
    else{
        TIM15->SR = ~TIM_SR_CC2IF; 										// Clear flag on cahnnel CC2
        TIM15->SR = ~TIM_SR_CC1IF; 										// Clear flag on cahnnel CC1
        TIM15->SR = ~TIM_SR_CC2OF; 										// Clear flag on cahnnel CC2
        TIM15->SR = ~TIM_SR_CC1OF; 										// Clear flag on cahnnel CC1
    }
}

void TIM3_IRQHandler(void) {
    TIM3->SR &= ~TIM_SR_UIF; 											// Clear UIF flag
    
    if (!(TIM3->SR & TIM_SR_CC2OF)&& !(TIM3->SR & TIM_SR_CC1OF)){
        
            if (TIM3->SR & TIM_SR_CC2IF)
            {
                TimeStamp3 = TIM3->CCR2;  								// IC2 capture pulse width measurement
                TIM3->SR = ~TIM_SR_CC2IF; 								// Clear flag on cahnnel CC2
                while (!(TIM3->SR & TIM_SR_CC1IF)); 					// Loop until the capture event flag is set
                TimeStamp4 = TIM3->CCR1; 								// IC1 capture period measurement
                TIM3->SR = ~TIM_SR_CC2IF; 								// Clear flag on cahnnel CC2                      
            }          
    }
            
    else{
        TIM3->SR = ~TIM_SR_CC2IF; 										// Clear flag on cahnnel CC2
        TIM3->SR = ~TIM_SR_CC1IF; 										// Clear flag on cahnnel CC1
        TIM3->SR = ~TIM_SR_CC2OF; 										// Clear flag on cahnnel CC2
        TIM3->SR = ~TIM_SR_CC1OF; 										// Clear flag on cahnnel CC1   
    }     
} 


void Initialize_TIM15 (void){ 											// Configure gpio pA3 pin as inputs

    
    
    __GPIOA_CLK_ENABLE(); 												// Enable gpioa clock
    GPIOA->MODER |= GPIO_MODER_MODER3_1; 								// Configure the I/O direction mode: alternate function mode.
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_1; 								// Pull-up: change with GPIO_PUPDR_PUPDR0_1 for pull-down 
    GPIOA->AFR[0] |= 0x00009000;
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN; 								// Initialize Timer(32bit) for an external up counter mode
    TIM15->CCMR1 |= 0x0102;
    TIM15->CCMR1 |= TIM_CCMR1_IC1F_0; 
    TIM15->CCMR1 |= TIM_CCMR1_IC1F_1;  									// Input caputre filter: 0011: fSAMPLING=fCK_INT, N=81011: fSAMPLING=fDTS/16, N=6. Write 0000: No filter
    TIM15->CCMR1 |= TIM_CCMR1_IC2F_0;
    TIM15->CCMR1 |= TIM_CCMR1_IC2F_1; 									// Input caputre filter: 0011: fSAMPLING=fCK_INT, N=81011: fSAMPLING=fDTS/16, N=6. Write 0000: No filter
    TIM15->CCER |= TIM_CCER_CC2P;
    TIM15->EGR = 0x0006; 												// Event generation. Configured as input
    TIM15->SMCR |= TIM_SMCR_SMS_2; 										// Slave mode selection: reset on rising edge
    TIM15->SMCR |= TIM_SMCR_TS_0;
    TIM15->SMCR |= TIM_SMCR_TS_2; 										// Filtered Timer Input 1 (TI1FP1)
    TIM15->CCER |= TIM_CCER_CC1E;  										// CC1 channel configured as input: capture enabled
    TIM15->CCER |= TIM_CCER_CC2E; 										// CC2 channel configured as input: capture enabled
    TIM15->PSC = 0x0001; 												// Prescaler [0+1]
    TIM15->ARR = 0xffff; 												// Max value for counter
    TIM15->SR = ~TIM_SR_CC1IF; 											// Clear flag on cahnnel CC1
    TIM15->SR = ~TIM_SR_CC2IF; 											// Clear flag on cahnnel CC2
    TIM15->DIER |= TIM_DIER_CC1IE;
    NVIC_SetPriority(TIM15_IRQn, 0); 									// Set interrupt to the highest priority
    NVIC_EnableIRQ(TIM15_IRQn);
    NVIC_SetVector(TIM15_IRQn, (uint32_t)TIM15_IRQHandler); 
    TIM15->CR2 |= TIM_CR2_TI1S; 										// The TIMx_CH1, CH2 and CH3 pins are connected to the TI1 input (XOR combination)
    TIM15->CR1 |= TIM_CR1_CEN;
    
}




void Initialize_TIM3 (void){ 											// Configure gpio pA6 pin as inputs

    
    
    __GPIOA_CLK_ENABLE(); 												// Enable gpioa clock                        
    GPIOA->MODER |= GPIO_MODER_MODER6_1; 								// Configure the I/O direction mode: alternate function mode.
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_1; 								// Pull-up: change with GPIO_PUPDR_PUPDR0_1 for pull-down 
    GPIOA->AFR[0] |= 0x02000000;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 								// Initialize Timer(32bit) for an external up counter mode
    TIM3->CCMR1 |= 0x0201;
    TIM3->CCMR1 |= TIM_CCMR1_IC1F_0; 
    TIM3->CCMR1 |= TIM_CCMR1_IC1F_1;  									// Input caputre filter: 0011: fSAMPLING=fCK_INT, N=81011: fSAMPLING=fDTS/16, N=6. Write 0000: No filter
    TIM3->CCMR1 |= TIM_CCMR1_IC2F_0;
    TIM3->CCMR1 |= TIM_CCMR1_IC2F_1; 									// Input caputre filter: 0011: fSAMPLING=fCK_INT, N=81011: fSAMPLING=fDTS/16, N=6. Write 0000: No filter
    TIM3->CCER |= TIM_CCER_CC2P;
    TIM3->EGR = 0x0006; 												// Event generation. Configured as input
    TIM3->SMCR |= TIM_SMCR_SMS_2; 										// Slave mode selection: reset on rising edge
    TIM3->SMCR |= TIM_SMCR_TS_0;
    TIM3->SMCR |= TIM_SMCR_TS_2; 										// Filtered Timer Input 1 (TI1FP1)
    TIM3->CCER |= TIM_CCER_CC1E;  										// CC1 channel configured as input: capture enabled
    TIM3->CCER |= TIM_CCER_CC2E; 										// CC2 channel configured as input: capture enabled
    TIM3->PSC = 0x0001; 												//Prescaler [0+1]
    TIM3->ARR = 0xffff; 												//Max value for counter
    TIM3->SR = ~TIM_SR_CC1IF; 											//Clear flag on cahnnel CC1
    TIM3->SR = ~TIM_SR_CC2IF; 											//Clear flag on cahnnel CC2
    TIM3->DIER |= TIM_DIER_CC1IE;
    NVIC_SetPriority(TIM3_IRQn, 0); 									// Set interrupt to the highest priority
    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetVector(TIM3_IRQn, (uint32_t)TIM3_IRQHandler); 
    TIM3->CR1 |= TIM_CR1_CEN;   
}
 
 
int main() {
    
    Serial pc(USBTX,USBRX);
    pc.baud(115200);
    pc.format(8,SerialBase::None,0);

    Initialize_TIM3();
    Initialize_TIM15();
    
    while(1) {
        
        TimeStamp1 = 0;
        TimeStamp2 = 0;
        TimeStamp3 = 0;
        TimeStamp4 = 0;
        
        wait_us(2000);
        
        percentage1 = TimeStamp1*10000/TimeStamp2;
        percentage2 = TimeStamp3*10000/TimeStamp4;
        
        Start1 = (1 >> 8)  & 255;
        Start2 = (255 >> 8)  & 255;
        Start3 = (4 >> 8)  & 255;
        
        byte1 = (percentage1 >> 8)  & 255;
        byte2 = percentage1 & 255;
        
        byte3 = (percentage2 >> 8)  & 255;
        byte4 = percentage2 & 255;


        if((percentage1>=0.0 && percentage1<=10000.0)&&(percentage2>=0.0 && percentage2<=10000.0)){
            
			//pc.putc(Start1); 
			//pc.putc(byte1);
			//pc.putc(byte2);
			//pc.putc(Start2);
			//pc.putc(byte3);
			//pc.putc(byte4); 
			//pc.putc(Start3);       

			printf("%d,%d\r\n", percentage1 , percentage2);
        }
        
    }

}
