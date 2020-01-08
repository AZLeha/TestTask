#ifndef UART_H_
#define UART_H_


#include <stdint.h>
#include <stdbool.h>


void UART_Init(void);
void UART_TransmitString(char* aData);
void UART_TransmitByte(uint8_t data);
bool UART_TransmitDataViaDMA(void * aData, uint16_t dataSize);


#endif /* UART_H_ */
