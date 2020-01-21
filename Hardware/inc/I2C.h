
#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>

/*! \todo Подлежит оптимизации:
 *  (1. реалзовать защиту от висяков.
 *   2. повешать на прерывания или ДМА
 *   3. кастомизировать под FreeRTOS )
*/


typedef enum
{
	eI2C_littleEndian =0,
	eI2C_bigEndian

}eI2C_Endianness;


void I2C_Init(void);

void I2C_WriteData (uint8_t ucDeviceAddress, uint8_t ucRegisterPointer, void * pvOutputData, uint8_t ucDataSize, eI2C_Endianness xDirectionData);
void I2C_ReadData (uint8_t ucDeviceAddress, uint8_t ucRegisterPointer, void * pvInputData, uint8_t ucDataSize, eI2C_Endianness xDirectionData);

#endif /* I2C_H_ */
