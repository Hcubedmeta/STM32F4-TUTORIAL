#include "stm32f4xx.h" // Device header

void delay(int t);

int main(void)
{
    // Enable clock GPIOC v� GPIOA
    RCC->AHB1ENR |= (1 << 2) | (1 << 0);

    // C?u h�nh PC13 l� Output Push-Pull
    GPIOC->MODER &= ~(3 << (13 * 2)); // X�a 2 bit mode PC13
    GPIOC->MODER |= (1 << (13 * 2));  // Output mode (01)
    GPIOC->OTYPER &= ~(1 << 13);      // Push-Pull
    GPIOC->OSPEEDR |= (3 << (13 * 2));// High speed
    GPIOC->PUPDR &= ~(3 << (13 * 2)); // No Pull-up/Pull-down

    // C?u h�nh PA0 l� Input v?i Pull-down
    GPIOA->MODER &= ~(3 << (0 * 2));  // Input mode (00)
    GPIOA->PUPDR |= (2 << (0 * 2));   // Pull-down (10)

    while (1)
    {
        if ((GPIOA->IDR & (1 << 0)) == 0) // Ki?m tra n?u n�t nh?n
        {
            GPIOC->BSRR = (1 << (13 + 16)); // T?t LED
            delay(200);
            GPIOC->BSRR = (1 << 13); // B?t LED
            delay(200);
        }
        else
        {
            GPIOC->BSRR = (1 << 29); // Gi? LED s�ng
        }
    }
}

// H�m delay
void delay(int t)
{
    for (int i = 0; i < 0x2FF; i++)
    {
        for (int j = 0; j < t; j++)
        {
            __asm("nop"); // L?nh no-operation gi�p t?i uu hon
        }
    }
}
