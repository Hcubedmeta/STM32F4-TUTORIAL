#include "stm32f4xx.h"                  // Device header
/*
&= : clear or delete 
|= : attach
^= ; toggle
Muon bat (1 bit cu the len 1) : D�ng |=
Muon tat (x�a bit ve 0) : D�ng &= ~
khi &= ~ clear bit muon clear va ko clear cac bit con lai
GPIOC->MODER &= ~(3<<(13*2)) clear 2 bit 26,27 cua chan 13 va cac bit khac van giu nguyen
00: Input (reset state)
01: General purpose output mode
10: Alternate function mode
11: Analog mode
GPIOC->OTYPER &= ~(1 << 13)  xoa bit cua chan 13 ve 0 tuc la mode output push-pull
GPIOC->OTYPER |= (1 << 13) bat bit cua chan 13 len 1 tuc la mode output opend-drain
0: Output push-pull (reset state)
1: Output open-drain
GPIOC->OSPEEDR |= (3 << (13*2)); // high speed
00: Low speed
01: Medium speed
10: Fast speed
11: High speed
GPIOC->PUPDR &= ~(3 << (13*2)); output push-pull ko can dung pull-up/pull-down
00: No pull-up, pull-down
01: Pull-up
10: Pull-down
11: Reserved
GPIOC->BSRR = (1<<29); la resets chan 13
Bits 31:16 BRy: Port x reset bit y (y = 0..15)
These bits are write-only and can be accessed in word, half-word or byte mode. A read to
these bits returns the value 0x0000.
0: No action on the corresponding ODRx bit
1: Resets the corresponding ODRx bit
GPIOC->BSRR =(1<<13); la sets chan 13
Bits 15:0 BSy: Port x set bit y (y= 0..15)
These bits are write-only and can be accessed in word, half-word or byte mode. A read to
these bits returns the value 0x0000.
0: No action on the corresponding ODRx bit
1: Sets the corresponding ODRx bit
*/
void delay(int time);
int main(void)
{
	RCC->AHB1ENR |= (1<<2); // clock GPIOC
	GPIOC->MODER &= ~(3<<(13*2)); // delete 2 bit 26,27 cua pc13 ve 0, 3 = 0b11 = (0xFFFF FF11 FFFF FFFF FFFF FFFF FFFF FFFF)
	GPIOC->MODER |= (1<<(13*2)); // set up 2 bit 26,27 cua pc13 ve 1
	GPIOC->OTYPER &= ~(1 << 13); // xoa bit chan 13 ve 0 tuc la mode ouput push-pull
	GPIOC->OSPEEDR |= (3 << (13*2)); // high speed
	GPIOC->PUPDR &= ~(3 << (13*2)); // no pull-up/pull-down
	while(1)
	{
//		GPIOC->ODR ^= (1<<13);
		GPIOC->BSRR = (1<<13 + 16);
		delay(200);
		GPIOC->BSRR = (1<<13);
		delay(200);
	}
}
void delay(int t){
	for(int i = 0;i<0x2ff;i++){
		for(int j = 0;j < t;j++)
		{}
	}
}
