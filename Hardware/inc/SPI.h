
#ifndef INC_SPI_H_
#define INC_SPI_H_
#include <stdint.h>



typedef enum
{
	eSPI_littleEndian =0,
	eSPI_bigEndian

}eSPI_Endianness;

void SPI_init(void);


void SPI_WriteData (void * pvOutputData, uint16_t usDataSize, eSPI_Endianness xDirectionData);
void SPI_ReadData (void * pvInputData, uint16_t usDataSize, eSPI_Endianness xDirectionData);



#endif /* INC_SPI_H_ */
