#include "dma.h"


#define DMA2EN	 		(1<<22)
#define DMA_SCR_EN 		(1<<0)
#define DMA_SCR_MINC	(1<<10)
#define DMA_SCR_PINC	(1<<9)
#define DMA_SCR_TCIE	(1<<4)
#define DMA_SCR_TEIE 	(1<<2)
#define DMA_SFCR_DMDIS 	(1<<2)

void dma2_mem2mem_config(void)
{
	//Enable clock to access to the DMA module
	RCC -> AHB1ENR |= DMA2EN;

	//Disable dma stream
	DMA2_Stream0 -> CR = 0;

	//Wait until stream is disabled
	while((DMA2_Stream0 -> CR & DMA_SCR_EN)){}

	//Configure dma parameter

	//Set MSIZE memory data size to half_word
	DMA2_Stream0 -> CR |= (1<<13);
	DMA2_Stream0 -> CR &= ~(1<<14);

	//Set Psize peripheral data size to half_word
	DMA2_Stream0 -> CR |= (1<<11);
	DMA2_Stream0 -> CR &= ~(1<<12);

	//Enable memory addr increment
	DMA2_Stream0 -> CR  |= DMA_SCR_MINC;

	//Enable peripheral addr increment
	DMA2_Stream0 -> CR |= DMA_SCR_PINC;

	//Select mem-to-mem transfer
	DMA2_Stream0 -> CR &= ~(1<<6);
	DMA2_Stream0 -> CR |= (1<<7);

	//Enable transfer complete interrupt
	DMA2_Stream0 -> CR |= DMA_SCR_TCIE;

	//Enable transfer error interrupt
	DMA2_Stream0 -> CR |= DMA_SCR_TEIE;

	//Disable direct mode
	DMA2_Stream0 -> FCR |= DMA_SFCR_DMDIS;

	//Set DMA FIFO threshold
	DMA2_Stream0 -> FCR |= (1<<0);
	DMA2_Stream0 -> FCR |= (1<<1);

	//Enable DMA interrupt in NVIC
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}


void dma_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len)
{
	//Set peripheral address
	DMA2_Stream0 -> PAR = src_buff;
	//Set memory address
	DMA2_Stream0 -> M0AR = dest_buff;
	//Set transfer length
	DMA2_Stream0 -> NDTR = len;
	//Enable dma stream
	DMA2_Stream0 -> CR |= DMA_SCR_EN;

}
