#include "stm32f4xx.h"
#include "uart.h"
#include <stdio.h>

#define GPIOCEN (1<<2)
#define PIN13 (1<<13)
#define BTN_PIN PIN13

uint8_t button_state;

int main()
{
	/*Enable clock access to GPIOC	 */
	RCC -> AHB1ENR |= GPIOCEN;

	//Set PC13 as input pin
	GPIOC -> MODER &= ~(1<<26);
	GPIOC -> MODER &= ~(1<<27);

	while(1)
	{
		//check if btn is pressed or not
		if (GPIOC -> IDR & BTN_PIN)
		{
			button_state = 0;
		}
		else
		{
			button_state = 1;
		}


		for(int i = 0; i< 90000; i++){}
	}
}
