#include "./inc/UART.h"

#include "stm32f10x.h"
#include <stdbool.h>
#include <stddef.h>

__attribute__((always_inline)) static inline void inlineTransmitByte(uint8_t data);



void UART_Init(void)
{
	//UART1 TX(PA9)  RX(PA10)

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;

	// \todo добавить формулу или дефаины для установки частот
	//0x1D4C 9600;
	// установлено 115200
	USART1->BRR = 0x270;


#if defined(UART_US_RX) && defined(UART_US_TX)

	GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_MODE10 |
					GPIO_CRH_CNF9 | GPIO_CRH_CNF10);
	GPIOA->CRH |= 	GPIO_CRH_MODE9 |
					GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_0;

	USART1->CR1 = USART_CR1_TE | USART_CR1_RE;


		#if defined(UART_US_RX_IRQ) && defined(UART_US_TX_IRQ)
			USART1->CR1 |= USART_CR1_RXNEIE|USART_CR1_TXEIE;
		#elif defined(UART_US_RX_IRQ)
			USART1->CR1 |= USART_CR1_RXNEIE;
		#elif defined(UART_US_TX_IRQ)
			USART1->CR1 |= USART_CR1_TXEIE;
	#endif


	#elif defined(UART_US_RX)
		GPIOA->CRH &= ~( GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
		GPIOA->CRH |=  GPIO_CRH_CNF10_0;

		USART1->CR1 =  USART_CR1_RE | USART_CR1_RXNEIE ;

		#ifdef UART_US_RX_IRQ
			USART1->CR1 |= USART_CR1_RXNEIE;
		#endif

	#elif defined(UART_US_TX)
		GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9 );
		GPIOA->CRH |= 	GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1;
		USART1->CR1 =  USART_CR1_TE;

		#ifdef UART_US_TX_IRQ
			USART1->CR1 |= USART_CR1_TXEIE;
		#endif

#endif




#if defined(UART_US_RX_IRQ) || defined(UART_US_TX_IRQ)
	#ifdef UART_US_RX_IRQ || UART_US_TX_IRQ
		NVIC_SetPriority(USART1_IRQn,UART_IRQ_Priority);
		NVIC_EnableIRQ (USART1_IRQn);
	#endif
#endif

	USART1->CR2 = 0;
	USART1->CR3 = 0;



#if defined(UART_US_DMA_TX) && defined(UART_US_TX)

	USART1->CR3 |= USART_CR3_DMAT; //select DMA for tx

	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	//4 channel
	DMA1_Channel4->CPAR =(uint32_t)(&USART1->DR);
	DMA1_Channel4->CCR = DMA_CCR4_PL | DMA_CCR4_MINC | DMA_CCR4_DIR | DMA_CCR4_TCIE;
	NVIC_SetPriority(DMA1_Channel4_IRQn,UART_DMA_Priority);
	NVIC_EnableIRQ (DMA1_Channel4_IRQn);

#endif


#if defined(UART_US_DMA_RX) && defined(UART_US_RX)

	USART1->CR3 |= USART_CR3_DMAR; //select DMA for RX

	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	//5 channel
	DMA1_Channel5->CPAR =(uint32_t)(&USART1->DR);
	DMA1_Channel5->CCR = DMA_CCR4_PL | DMA_CCR4_MINC | DMA_CCR4_TCIE;
	NVIC_SetPriority(DMA1_Channel5_IRQn,UART_DMA_Priority);
	NVIC_EnableIRQ (DMA1_Channel5_IRQn);

#endif

	USART1->CR1 |= USART_CR1_UE;

}









//=========================================UART_US_TX===========================================
//______________________________________________________________________________________________
#ifdef  UART_US_TX
	// \todo можно добавить отключение этих функций по желанию

	#ifdef  UART_US_TX_IRQ
		UART_Colback _IRQTxHandler = NULL;
		void UART_setIRQTxHandler(UART_Colback xHandler)
		{
			_IRQTxHandler = xHandler;
		}
	#endif
	void UART_TransmitString(char* acData)
	{

		while((*acData) != '\0')
		{
			inlineTransmitByte((uint8_t)(*acData));
			acData++;
		}


	}

	void UART_TransmitByte(uint8_t data)
	{
		// \todo Нужно добавить проверку на флаг ошибок, чтобы не было висяков
		while(!(USART1->SR &= USART_SR_TXE)) asm("NOP");
		USART1->DR = data;
	}




	__attribute__((always_inline))  inline void inlineTransmitByte(uint8_t data)
	{
		// \todo Нужно добавить проверку на флаг ошибок, чтобы не было висяков
		while(!(USART1->SR &= USART_SR_TXE)) asm("NOP");
		USART1->DR = data;
	}



#endif
//______________________________________________________________________________________________












//=========================================UART_US_TX===========================================
//______________________________________________________________________________________________
#ifdef  UART_US_RX
	#ifdef  UART_US_RX_IRQ
		UART_Colback _IRQRxHandler = NULL;
		void UART_setIRQRxHandler(UART_Colback xHandler)
		{
			_IRQRxHandler = xHandler;
		}
	#endif


#endif
//______________________________________________________________________________________________








//==========================UART_US_TX_IRQ or UART_US_RX_IRQ====================================
//______________________________________________________________________________________________

#if defined(UART_US_TX_IRQ) || defined(UART_US_RX_IRQ)

void USART1_IRQHandler()
{



	#ifdef  UART_US_RX_IRQ
		if(USART1->SR & USART_SR_RXNE)
		{
			if(_IRQRxHandler != NULL ) _IRQRxHandler();
		}
	#endif

	#ifdef  UART_US_TX_IRQ

		if(USART1->SR & USART_SR_TXE)
		{
			if(_IRQTxHandler != NULL ) _IRQTxHandler();
		}
	#endif

	USART1->SR = 0;
	//UART_TransmitByte(USART1->DR+1);

}

#endif
//______________________________________________________________________________________________




//=======================================UART_US_DMA_TX=========================================
//______________________________________________________________________________________________
#ifdef  UART_US_DMA_TX

	bool isDMAFree = true;

	UART_Colback _DMATxHandler = NULL;
	void UART_setDMATxHandler(UART_Colback xHandler)
	{
		_DMATxHandler = xHandler;
	}


	bool UART_TransmitDataViaDMA(void * aData, uint16_t dataSize)
	{
		if(!isDMAFree) return false;
		isDMAFree = false;

		while(DMA1_Channel4->CCR & DMA_CCR1_EN) asm("nop");

		DMA1_Channel4->CNDTR = dataSize;
		DMA1_Channel4->CMAR = (uint32_t)aData;

		DMA1_Channel4->CCR |= DMA_CCR4_EN;


		return true;
	}


	void DMA1_Channel4_IRQHandler()
	{
		DMA1->IFCR=DMA_IFCR_CTCIF4;
		DMA1_Channel4->CCR&=~DMA_CCR1_EN;
		isDMAFree=true;
		if(_DMATxHandler != NULL) _DMATxHandler();
	}

#endif
//______________________________________________________________________________________________





//=======================================UART_US_DMA_RX=========================================
//______________________________________________________________________________________________
#ifdef  UART_US_DMA_RX

	UART_Colback _DMARxHandler = NULL;
	void UART_setDMARxHandler(UART_Colback xHandler)
	{
		_DMARxHandler = xHandler;
	}


	bool UART_ReceiveDataViaDMA(void * aData, uint16_t dataSize)
	{

		if(DMA1_Channel4->CCR & DMA_CCR1_EN)return false;


		DMA1_Channel5->CNDTR = dataSize;
		DMA1_Channel5->CMAR = (uint32_t)aData;

		DMA1_Channel5->CCR |= DMA_CCR4_EN;

		return true;
	}


	bool UART_StopReceiveViaDMA()
	{
		DMA1_Channel5->CCR &= ~DMA_CCR4_EN;

		if(DMA1_Channel4->CCR & DMA_CCR1_EN)return false;

		return true;
	}


void DMA1_Channel5_IRQHandler()
	{
		DMA1->IFCR = DMA_IFCR_CTCIF5;
		DMA1_Channel5->CCR&=~DMA_CCR1_EN;

		if(_DMARxHandler != NULL) _DMARxHandler();
	}

#endif
//______________________________________________________________________________________________








