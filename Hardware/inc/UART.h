#ifndef UART_H_
#define UART_H_


#include <stdint.h>
#include <stdbool.h>

#include "stm32f10x.h"



//============================================Configure=========================================
//______________________________________________________________________________________________
#define UART_US_RX
#ifdef  UART_US_RX
	//#define UART_US_DMA_RX
	#define UART_US_RX_IRQ
#endif


#define UART_US_TX
#ifdef  UART_US_TX
	#define UART_US_DMA_TX
	//#define UART_US_TX_IRQ
#endif


#if defined(UART_US_TX_IRQ) || defined(UART_US_RX_IRQ)
	#define UART_IRQ_Priority 3
#endif

#if defined(UART_US_DMA_TX) || defined(UART_US_DMA_RX)
	#define UART_DMA_Priority 3
#endif
//______________________________________________________________________________________________










typedef void (*UART_Colback)(void);

void UART_Init(void);
void UART_TransmitString(char* aData);
void UART_TransmitByte(uint8_t data);

__attribute__((always_inline)) inline uint8_t UART_ReadByte()
{
	return (uint8_t)USART1->DR;
}



bool UART_TransmitDataViaDMA(void * aData, uint16_t dataSize);
bool UART_ReceiveDataViaDMA(void * aData, uint16_t dataSize);
bool UART_StopReceiveViaDMA();



void UART_setDMATxHandler(UART_Colback xHandler);
void UART_setDMARxHandler(UART_Colback xHandler);

void UART_setIRQTxHandler(UART_Colback xHandler);
void UART_setIRQRxHandler(UART_Colback xHandler);

// \todo Запилить обработчик ошибок
void UART_ErorHandler();




#endif /* UART_H_ */
