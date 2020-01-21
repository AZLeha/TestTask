#include "./inc/SPI.h"
#include "stm32f10x.h"

__attribute__((always_inline)) static inline uint8_t SPI_transceiver(uint8_t data);

void SPI_init()
{

	//PA5 CLK
	//PA6 MISO
	//PA7 MOSI

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;



	GPIOA->CRL|= GPIO_CRL_MODE5 | GPIO_CRL_MODE7;
	GPIOA->CRL &= ~GPIO_CRL_MODE6;

	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);
	GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_1;

	SPI1->CR1 = SPI_CR1_BR | SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI;
	SPI1->CR1 |= SPI_CR1_SPE;
}


void SPI_WriteData (void * pvOutputData, uint16_t usDataSize, eSPI_Endianness xDirectionData)
{
	uint8_t *pointer = pvOutputData;

	for(uint16_t i = 0; usDataSize > 0; i++, usDataSize--)
	{
		if(xDirectionData == eSPI_bigEndian)
			SPI_transceiver(pointer[usDataSize-1]);
		else
			SPI_transceiver(pointer[i]);
	}
}

void SPI_ReadData ( void * pvInputData, uint16_t usDataSize, eSPI_Endianness xDirectionData)
{
	uint8_t *pointer = pvInputData;
	for(uint16_t i = 0; usDataSize > 0; i++, usDataSize--)
	{
		if(xDirectionData == eSPI_bigEndian)
			pointer[usDataSize-1] = SPI_transceiver(0xFF);
		else
			pointer[i] = SPI_transceiver(0xFF);
	}
}






__attribute__((always_inline)) inline uint8_t SPI_transceiver(uint8_t data)
{
	SPI1->DR = data;

	while (SPI1->SR & SPI_SR_BSY) asm("nop");

	return (uint8_t)SPI1->DR;
}
