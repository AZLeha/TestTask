#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "UART.h"

void blink(void *par);

int main(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	GPIOC->CRH |= GPIO_CRH_MODE13;
	GPIOC->CRH &= ~GPIO_CRH_CNF13;

	UART_Init();

	UART_TransmitString("\033[2J");
	UART_TransmitString("Test\n\r");

	xTaskCreate(blink,(char*)"blink",100,NULL,tskIDLE_PRIORITY+1,NULL);

	vTaskStartScheduler();

	while(1)
	{

	}
	return 0;
}

void blink(void *par)
{

	while(1)
	{
		GPIOC->BSRR=GPIO_BSRR_BR13;
		vTaskDelay(500);
		GPIOC->BSRR=GPIO_BSRR_BS13;
		vTaskDelay(500);
	}

}
