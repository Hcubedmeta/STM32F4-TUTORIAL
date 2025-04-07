#include "stm32f4xx.h"                  // Device header
#include "H_SPI.h"
#include "ILI9225.h"
#include "stdio.h"

uint16_t Read_ADC(uint8_t channel) ;
void ADC_init(void);
uint16_t Read_ADC_Avg(uint8_t channel);
int main(void)
{
	char x;
	char y;
	char prev_x = WIDTH /2, prev_y = HEIGHT/2;
	ADC_init();
	SPI_init(SPI_2);
	lcd_init();
	fill_rectangle(0,0,WIDTH,HEIGHT,COLOR_BLACK);

	while(1)
	{
		uint16_t adc_x = Read_ADC_Avg(4);
		uint16_t adc_y = Read_ADC_Avg(5);

		// Chuy?n d?i ADC thành t?a d? màn hình
		x = ((adc_x * WIDTH) / 3450);
		y = ((adc_y * HEIGHT) / 3450);

		// Gi?i h?n giá tr? x, y trong màn hình
		if (x >= WIDTH - 10) x = WIDTH - 6 ;
		if (x < 5) x = 5;

		if (y >= HEIGHT - 10) y = HEIGHT -6 ;
		if (y < 5) y = 5;

		// Xóa con tr? cu dúng cách
		fill_rectangle(prev_x - 5, prev_y - 5, prev_x + 5, prev_y + 5, COLOR_BLACK);

		// V? con tr? m?i
		drawCircle(x, y, 5, COLOR_RED, 4);
		

		// C?p nh?t v? trí m?i
		prev_x = x;
		prev_y = y;
		delay_ms(1);

	}
}
void ADC_init(void)
{
	// 1. B?t clock cho ADC1 v? GPIOA
	RCC->APB2ENR |= (1 << 8);  // B?t clock cho ADC1
	RCC->AHB1ENR |= (1 << 0);  // B?t clock cho GPIOA

	// 2. C?u h?nh PA4 v? PA5 ? ch? d? Analog
	GPIOA->MODER |= (3 << (4 * 2)) | (3 << (5 * 2)); 

	// 3. C?u h?nh ADC1
	ADC1->CR1 |= (1 << 8);    // B?t ch? d? qu?t (SCAN mode)
	ADC1->CR2 |= (1 << 1);    // B?t ch? d? li?n t?c (Continuous mode)

	// 4. Ch?n s? k?nh chuy?n d?i
	ADC1->SQR1 |= (1 << 20);  // ??t s? k?nh c?n qu?t (L = 1, t?c l? 2 k?nh)


	// 6. B?t ADC
	ADC1->CR2 |= (1 << 0);    // B?t ADC

	// 7. B?t d?u chuy?n d?i ADC
	ADC1->CR2 |= (1 << 30);   // B?t d?u chuy?n d?i ADC

}
uint16_t Read_ADC(uint8_t channel) {
    // Ch?n kênh ADC
    ADC1->SQR3 = channel;  

    // B?t d?u chuy?n d?i
    ADC1->CR2 |= ADC_CR2_SWSTART;  

    // Ch? chuy?n d?i hoàn t?t
    while (!(ADC1->SR & ADC_SR_EOC));  

    return (uint16_t)ADC1->DR;  // Tr? v? giá tr? ADC
}
uint16_t Read_ADC_Avg(uint8_t channel) {
    uint32_t sum = 0;
    for (int i = 0; i < 18; i++) {
        sum += Read_ADC(channel);
    }
    return (uint16_t)sum / 18;  // L?y trung bình 10 l?n d?c
}
