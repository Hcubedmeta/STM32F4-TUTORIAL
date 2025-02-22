#include "stm32f4xx.h"                  // Device header
/* setup EXTI at Px (x = A,B,C,D,..)
	SYSCFG_EXTICR1 = pin [0 - 3] 
	SYSCFG_EXTICR2 = pin [4 - 7]
	SYSCFG_EXTICR3 = pin [8 - 11]
	SYSCFG_EXTICR4 = pin [12 - 15]
	
	SYSCFG->EXTICR[0] &= ~(0xF << (0 * 4));   one F is equal to 4 bits. ex F = 1111. 
	with this code  &= ~ : clear 4 bits into 0000,so it's PA(0000).
	external interrupt.
	0000: PA[x] pin
	0001: PB[x] pin
	0010: PC[x] pin
	0011: PD[x] pin
	0100: PE[x] pin
	0101: Reserved
	0110: Reserved
	0111: PH[x] pin
 	*/
void EXTI0_Init(void);
void EXTI0_IRQHandler(void);
void delay(int t);
int main(void)
{
	EXTI0_Init(); 
	// config PC13
	RCC->AHB1ENR |= (1 << 2); // enable clock PC
	GPIOC->MODER &= ~(1 << (13 * 2)); // clear bit 26,27 cua chan pc13
	GPIOC->MODER |= (1 << (13 * 2)); // attached bit 26,27 cua chan pc13
	GPIOC->OTYPER &= ~(1 << 13); // mode output push-pull
	GPIOC->OSPEEDR |= (1 << (13*2)); // high speed
	GPIOC->PUPDR &= ~(1 << (13 * 2)); // no pull-up/pull-down
	while(1)
	{
		
	}
}
void EXTI0_Init(void)
{ 
	RCC->AHB1ENR |= (1<<0); // enable clock PA
	RCC->APB2ENR |= (1<<14); // enable syscfg
	GPIOA->MODER &= ~(3 << (2*0)); // clear bit 0,1 cua pa0 tuc la mode input
	SYSCFG->EXTICR[0] &= ~(0xF << (0 * 4)); // attached exti0 cho pa 0
	EXTI->IMR |= (1 << 0); // interrupt is not masked
	EXTI->EMR |= (1 << 0); // event intterupt is not masked
	EXTI->FTSR |= (1 << 0); // falling edge
	NVIC_SetPriority(EXTI0_IRQn,2); // mode set priotity
	NVIC_EnableIRQ(EXTI0_IRQn); // enable interrup 
}
void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & (1 << 0)) // check interrupt flag
	{
		EXTI->PR |= (1 << 0); // clear interrupt flag
		GPIOC->ODR ^= (1 << 13);

	}
}
void delay(int t)
{
	for(int i = 0;i < 0x2ff;i++)
	{
		for (int j = 0;j < t;j++)
		{ }
	}
}
