#include "stm32f4xx.h"
#include "uart.h"
#include <stdio.h>

int main()
{
	uart2_tx_init();
	while(1)
	{
		printf("Hello from main function \n\r");


	}
}
