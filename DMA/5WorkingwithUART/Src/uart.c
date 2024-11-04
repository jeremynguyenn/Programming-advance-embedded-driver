#include "uart.h"

#define UART2EN (1<<17)
#define GPIOAEN (1<<0)
#define CR1_TE	(1<<3)
#define CR1_RE	(1<<2)
#define CR1_UE	(1<<13)
#define SR_TXE	(1<<7)

#define UART_BAUDRATE 	115200
#define CLK 		 	16000000

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart2_write(int ch);

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

void uart2_tx_init(void)
{
	/*Configure UART GPIO pin*/
	//Enable clock access to GPIOA
	RCC -> AHB1ENR |= GPIOAEN;
	//Set PA2 mode to alternate function mode
	GPIOA -> MODER &= ~(1<<4);
	GPIOA -> MODER |= (1<<5);
	//Set PA2 alternate function type AF7(UART2_TX)
	GPIOA -> AFR[0] |= (1<<8);
	GPIOA -> AFR[0] |= (1<<9);
	GPIOA -> AFR[0] |= (1<<10);
	GPIOA -> AFR[0] &= ~(1<<11);

	/*Configure UART module	*/
	//Enable clock to access UART2
	RCC -> APB1ENR |= UART2EN;
	//Set baudrate
	uart_set_baudrate(CLK, UART_BAUDRATE);
	//Set transfer direction
	USART2 -> CR1 = CR1_TE;
	//Enable uart module
	USART2 -> CR1 |= CR1_UE;

}


static void uart2_write(int ch)
{
	//Make sure the transmit data register is empty
	while (!(USART2 -> SR & SR_TXE)){}
	//Write to transmit data register
	USART2-> DR = (ch & 0xFF);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk +(baudrate/2U))/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2 -> BRR = compute_uart_bd(periph_clk, baudrate);
}
