
#include "stm32f10x.h"
#include <stdbool.h>


#include "inc/AT45DB641E.h"
#include "SPI.h"


#ifdef  UART_DEBUG
#include "UART.h"
#include <stdio.h>
#endif


#define AT45DB641E_REST(isRest) (GPIOA->BSRR = isRest ? GPIO_BSRR_BR2 : GPIO_BSRR_BS2)
#define AT45DB641E_CS(isCS) (GPIOA->BSRR = isCS ? GPIO_BSRR_BR4 : GPIO_BSRR_BS4)
#define AT45DB641E_WP(isWP) (GPIOA->BSRR = isWP ? GPIO_BSRR_BR3 : GPIO_BSRR_BS3)


#define MMPR 	0xD2 //Main Memory Page Read
#define CARHF 	0x0B //Continuous Array Read (High Frequency)
#define CARHF_M 0x1B //Continuous Array Read (High Frequency) (this is max speed)
#define MMPPTB1 0x82 //Main Memory Page Program through Buffer 1 with Built-In Erase
#define MMPPTB2 0x85 //Main Memory Page Program through Buffer 2 with Built-In Erase
#define CE		0x9A8094C7 //Chip Erase
#define SRR		0xD7 //Status Register Read

void AT45DB641E_configurePageSize(AT45DB641E_ePageSize xPageSize)
{
	//Table 11-1. Buffer and Page Size Configuration Commands
	const uint32_t  binarySizeComand = 0xA6802A3D;
	const uint32_t  defaultSizeComand = 0xA7802A3D;

	if(xPageSize == AT45DB641E_eBinarysize)	SPI_WriteData(&binarySizeComand, 4, eSPI_littleEndian);
	else SPI_WriteData(&defaultSizeComand, 4, eSPI_littleEndian);
}

typedef union {

	//тут небольшой кастыль так как сперва нужно отправит старший байт адреса а потом младщий
	//то заголовок будет пеердоватсяв обратно пордке
	#pragma pack(push, 1)
	struct
	{
		uint32_t 	uDummybByte;	//length dummyByte from one to four byte
		uint8_t 	ucByteAddress;	//Starting byte number of the selected sector
		uint16_t 	usSectorNumber;	//Number of sector in the memory table
		uint8_t		ucOpcode;		//opcode
	};
	#pragma pack(pop)
	uint8_t array[8];
}PacketHeder;


void AT45DB641E_init(void)
{
	SPI_init();
	//PA4 CS
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL|= GPIO_CRL_MODE4 | GPIO_CRL_MODE3 | GPIO_CRL_MODE2;
	GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_CNF3 | GPIO_CRL_CNF2);

	AT45DB641E_CS(false);
	AT45DB641E_WP(false);

	AT45DB641E_REST(true);
	for(uint32_t i=0; i<0xFffff;i++) asm("NOP");
	AT45DB641E_REST(false);

}

void AT45DB641E_readSector(void * pvInputData, uint16_t usSectorNumber)
{
	//Header packet consist of opcode 15 bite number of sector 9 bite address in the sector and two dummy byte
	//[opcode(0-7bits)][ (8bits MSB byte address) (9-23 bits number sector) ] [LSB byte address (24-31 bits)] [dummy byte (32-47 bites)]
 	PacketHeder heder = {{.ucOpcode = CARHF_M, .usSectorNumber = usSectorNumber<<1}};

 	AT45DB641E_CS(true);
	SPI_WriteData(heder.array+2, 6, eSPI_bigEndian);
	SPI_ReadData(pvInputData, SECTOR_SIZE, eSPI_littleEndian);
	AT45DB641E_CS(false);

}
void AT45DB641E_writeSector(void * pvOutputData, uint16_t usSectorNumber)
{
	//Header packet consist of opcode two byte number of
	//sector one byte address
	PacketHeder heder = {{.ucOpcode = MMPPTB1, .usSectorNumber = usSectorNumber<<1}};

	//ТУТ МОГЛА БЫТЬ ОПТИМИЗАЦИЯ ДЛЯ ЗАПИСИ ЧЕРЗ 2 БУФЕРА НО её НЕТ ))
	AT45DB641E_CS(true);
	SPI_WriteData(heder.array+4, 4, eSPI_bigEndian);
	SPI_WriteData(pvOutputData, SECTOR_SIZE, eSPI_littleEndian);
	AT45DB641E_CS(false);


	for(uint32_t i=0; (i<0x1fffff) && (!(AT45DB641E_status()&(1<<7)));i++) asm("NOP");
}

uint16_t AT45DB641E_status(void)
{
	uint16_t status = SRR;

	AT45DB641E_CS(true);
	SPI_WriteData(&status, 1,eSPI_littleEndian);
	SPI_ReadData(&status, 2, eSPI_littleEndian);
	AT45DB641E_CS(false);

	return status;
}



