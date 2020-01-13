
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
	eI2C_MSBFirst =0,
	eI2C_MSBLast

}eI2C_directionData;


void I2C_Init(void);

void I2C_WriteData (uint8_t ucDeviceAddress, uint8_t ucRegisterPointer, void * pvOutputData, uint8_t ucDataSize, eI2C_directionData xDirectionData);
void I2C_ReadData (uint8_t ucDeviceAddress, uint8_t ucRegisterPointer, void * pvInputData, uint8_t ucDataSize, eI2C_directionData xDirectionData);

#endif /* I2C_H_ */
