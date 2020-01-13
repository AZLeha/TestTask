#include "stm32f10x.h"
/*
#include "FreeRTOS.h"
#include "task.h"*/


#include <stdio.h>

#include "UART.h"
#include "BMP280.h"



void blink(void *par);


int main(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	GPIOC->CRH |= GPIO_CRH_MODE13;
	GPIOC->CRH &= ~GPIO_CRH_CNF13;

	UART_Init();

	UART_TransmitString("\033[2J");
	UART_TransmitString("Start\n\r");




	BMP280_init();
	BMP280_setings(	eBMP280_setings_osrs_p16 |
					eBMP280_setings_osrs_tX2 |
					eBMP280_setings_filterX16 |
					eBMP280_setings_t_sb0_5 |
					eBMP280_setings_modeNormal
					);

	double temp=0, press=0;







/*



	xTaskCreate(blink,(char*)"blink",100,NULL,tskIDLE_PRIORITY+1,NULL);
	vTaskStartScheduler();*/

	while(1)
	{
		temp = BMP280_getTemp();
		press = BMP280_getPress();

		char array[100];
		//sprintf (array, "data is read 0x%x", temp);
		//sprintf (array, "data is read %d", temp);



		UART_TransmitString("\033[2J");
		UART_TransmitString("Start\n\r");

		sprintf (array, "Temp = %lf\n\r", temp);
		UART_TransmitString(array);

		sprintf (array, "Pressure = %lf", press);
		UART_TransmitString(array);

		GPIOC->BSRR=GPIO_BSRR_BR13;
		for(uint32_t i=0; i<0xfffff;i++) asm("NOP");
		GPIOC->BSRR=GPIO_BSRR_BS13;
		for(uint32_t i=0; i<0xfffff;i++) asm("NOP");

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
