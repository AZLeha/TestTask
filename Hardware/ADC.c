#include "inc/ADC.h"

#include "stm32f10x.h"
#include "UART.h"
#include "stddef.h"
ADC_Colback handler = NULL;

void ADC_init(uint16_t usHThreshold, uint16_t usLThreshold)
{

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;

	GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_CNF0 | GPIO_CRL_CNF1);

	ADC1->CR2 = 0;

	ADC1->HTR = usHThreshold;
	ADC1->LTR = usLThreshold;

	ADC1->CR1 |=  ADC_CR1_JAWDEN | ADC_CR1_JAUTO | ADC_CR1_AWDSGL | ADC_CR1_SCAN | ADC_CR1_AWDIE | ADC_CR1_AWDCH_0;

	ADC1->CR2 |= ADC_CR2_JEXTTRIG | ADC_CR2_JEXTSEL | ADC_CR2_CONT;


	ADC1->JSQR = ADC_JSQR_JL_0 | ADC_JSQR_JSQ4_0 ;


	ADC1->SMPR2 = ADC_SMPR2_SMP1 | ADC_SMPR2_SMP2;

	//калибровка

	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_CAL;
	while (ADC1->CR2 & ADC_CR2_CAL) asm("NOP");

	ADC1->CR2 |= ADC_CR2_JSWSTART;

	ADC1->SR = 0;


	NVIC_SetPriority(ADC1_2_IRQn,4);
	NVIC_EnableIRQ (ADC1_2_IRQn);
}

uint16_t ADC_getData(void)
{
	return (uint16_t)ADC1->JDR1;
}

void ADC_setWatchdogHandler(ADC_Colback xHandler)
{
	handler=xHandler;
}

void ADC1_2_IRQHandler(void)
{
	if(ADC1->SR & ADC_SR_AWD)
	{
		if(handler != NULL) handler();
	}
	ADC1->SR = 0;
}

