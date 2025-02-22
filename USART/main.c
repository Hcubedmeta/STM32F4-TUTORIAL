#include "stm32f4xx.h"                  // Device header
/*
there are steps to config usart
1: enable clock usart and GPIO
2: config GPIO of usart 
3: set baudrate by fomular : Usartdiv = Fclk/baud rate (Fclk is 8Mhz or 16Mhz)
4: config usart
5: transmit a character
6: receive a character
AF7 : USART1 and USART2
AF8 : USART 6
USART1 : pa9 = tx,pa10 =rx or pb6 = tx,pb7 = rx
USART2 : pa2 = tx,pa3 = rx
USART6 : pa11 = tx,pa12 =rx
AFR[0] : pin0-7
AFR[1] : pin8-15
*/
void USART2_Config(void);
char data;
int main(void)
{
	USART2_Config();
	RCC->AHB1ENR |= (1<<2); // clock GPIOC
	GPIOC->MODER &= ~(3<<(13*2)); // delete 2 bit 26,27 cua pc13 ve 0, 3 = 0b11 = (0xFFFF FF11 FFFF FFFF FFFF FFFF FFFF FFFF)
	GPIOC->MODER |= (1<<(13*2)); // set up 2 bit 26,27 cua pc13 ve 1
	GPIOC->OTYPER &= ~(1 << 13); // xoa bit chan 13 ve 0 tuc la mode ouput push-pull
	GPIOC->OSPEEDR |= (3 << (13*2)); // high speed
	GPIOC->PUPDR &= ~(3 << (13*2)); // no pull-up/pull-down
	while(1)
	{
    if (USART2->SR & (1 << 5)) {  // check  RXNE has new anydata 
        char data = USART2->DR;   // read and write data in DR register
        if (data == 'B') 
				{  
            GPIOC->BSRR = (1 << 29);  // B?t LED (PC13)
            while (!(USART2->SR & (1 << 7)));  // waiting data will ready
            USART2->DR = 'B';  // feedback character 'B' for usart 
				}
				if (data == 'T') 
				{  
						GPIOC->BSRR = (1 << 13);  // B?t LED (PC13)
						while (!(USART2->SR & (1 << 7)));  // waiting data will ready
						USART2->DR = 'T';  // feedback character 'T' for usart
				}
    }
  }
}
void USART2_Config(void)
{
	RCC->APB1ENR |= (1 << 17); // enable clock USART2
	RCC->AHB1ENR |= (1 << 0); // enable clock GPIOA
	GPIOA->MODER &= ~(3 << (2 * 2) | 3 << (2 * 3)); // clear bit 4,5 va 6,7 chan pa2,pa3
	GPIOA->MODER |= (2 << (2 * 2) | 2 << (2 * 3)); //attached bit cua pa1,pa3 mode alternate fuction
	GPIOA->AFR[0] |= (7 << 8 | 7 << 12); // AFRL : AFR[0] attached bit 8 - 11 of pa2,12-15 of pa3 for AF7
	USART2->BRR |= 0x0683; // baudrate 9600 with Fclk : 16Mhz
	USART2->CR1 |= (1 << 2 | 1 << 3); // setup tx,rx
	USART2->CR1 |= (1 << 13); // enable usart
	USART2->CR1 |= (1 << 12); // transmit 8bit
}
