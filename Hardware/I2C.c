#include "./inc/I2C.h"

#include "stm32f10x.h"

#define  FREQR 2000000 //I2C clock frequency in Hz
#define  SCL_CLOCK 100 //SCL clock frequency in KHz



typedef enum
{
	eRead =0,
	eWrite
}eDataDirection;



__attribute__((always_inline)) static inline void inlineStartConditions(uint8_t ucDeviceAddress,eDataDirection xDataDirection);

__attribute__((always_inline)) static inline void inlineWriteData(void * pvOutputData, uint8_t ucDataSize, eI2C_directionData xDirectionData);
__attribute__((always_inline)) static inline void inlineReadData(void * pvInputData, uint8_t ucDataSize, eI2C_directionData xDirectionData);




void I2C_Init()
{

	// PB6 I2C1_SCL    PB7 I2C1_SDA
	RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7;


	I2C1->CR1 = 0;

	I2C1->CR2 = SystemCoreClock / FREQR;

	//500 магическое чило 0,5(так-как нужно пол периуда) *1000(так-как расчёт в KHz )
	I2C1->CCR = (uint32_t)(500 / SCL_CLOCK / ((float)1 / (SystemCoreClock / FREQR)));

	I2C1->TRISE = (SystemCoreClock / FREQR) + 1;

	I2C1->CR1 |= I2C_CR1_PE;

}


void I2C_WriteData (uint8_t ucDeviceAddress, uint8_t ucRegisterPointer, void * pvOutputData, uint8_t ucDataSize, eI2C_directionData xDirectionData)
{
	inlineStartConditions(ucDeviceAddress,eWrite);
	inlineWriteData(&ucRegisterPointer, 1, xDirectionData);
	inlineWriteData(pvOutputData, ucDataSize, xDirectionData);
	I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_ReadData (uint8_t ucDeviceAddress, uint8_t ucRegisterPointer, void * pvInputData, uint8_t ucDataSize, eI2C_directionData xDirectionData)
{
	inlineStartConditions(ucDeviceAddress,eWrite);
	inlineWriteData(&ucRegisterPointer, 1, xDirectionData);
	inlineStartConditions(ucDeviceAddress,eRead);
	inlineReadData(pvInputData,ucDataSize, xDirectionData);
}




/*! \todo Подлежит оптимизации:
 *  (1. реалзовать защиту от висяков.)
 */
__attribute__((always_inline)) inline void inlineStartConditions(uint8_t ucDeviceAddress, eDataDirection xDataDirection)
{

	I2C1->CR1 |= I2C_CR1_START  ;
	while (!(I2C1->SR1&I2C_SR1_SB))asm("NOP");
	ucDeviceAddress <<=1;

	if(xDataDirection == eRead) ucDeviceAddress |= 1;

	I2C1->DR = (uint32_t)ucDeviceAddress;
	while (!(I2C1->SR1 & I2C_SR1_ADDR)){asm("NOP");}
	(void) I2C1->SR2;	//костыль в мануале написано This bit is cleared by software reading SR1 register followed reading SR2, or by hardware when PE=0.
	//но так как перефирию отрубать не в прикол пилим так
}


/*! \todo Подлежит оптимизации:
 *  (1. реалзовать защиту от висяков.)
 */
__attribute__((always_inline)) inline void inlineWriteData(void * pvOutputData, uint8_t ucDataSize, eI2C_directionData xDirectionData)
{
	uint8_t *pointer = pvOutputData;


	for(uint8_t i = 0;ucDataSize>0; i++, ucDataSize--)
	{
		if(xDirectionData == eI2C_MSBLast)
			I2C1->DR = (uint16_t) pointer[i];
		else
			I2C1->DR = (uint16_t) pointer[ucDataSize-1];

		while (!(I2C1->SR1 & I2C_SR1_TXE)){asm("NOP");}
	}
}

/*! \todo Подлежит оптимизации:
 *  (1. реалзовать защиту от висяков.)
 */
__attribute__((always_inline)) inline void inlineReadData(void * pvInputData, uint8_t ucDataSize, eI2C_directionData xDirectionData)
{
	uint8_t *pointer = pvInputData;

	I2C1->CR1 |= I2C_CR1_ACK;
	for(uint8_t i = 0;ucDataSize>0; i++, ucDataSize--)
	{

		if(ucDataSize==1)	//если последний байт то не генреируем ACK и следом выстовляем на шину стоп условие
		{
			I2C1->CR1 &= ~I2C_CR1_ACK;
			I2C1->CR1 |= I2C_CR1_STOP;
		}

		while (!(I2C1->SR1 & I2C_SR1_RXNE)){asm("NOP");}

		if(xDirectionData == eI2C_MSBLast)
			pointer[i] = (uint8_t)I2C1->DR;
		else
			pointer[ucDataSize-1] = (uint8_t)I2C1->DR;

		pvInputData++;
	}

}




