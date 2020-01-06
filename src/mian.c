#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"

void blink(void *par);

int main(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	GPIOC->CRH |= GPIO_CRH_MODE13;
	GPIOC->CRH &= ~GPIO_CRH_CNF13;


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
		vTaskDelay(100);
		GPIOC->BSRR=GPIO_BSRR_BS13;
		vTaskDelay(100);
	}

}
