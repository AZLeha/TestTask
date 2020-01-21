
#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"


#include <stdio.h>

#include "UART.h"
#include "BMP280.h"
#include "ff.h"
#include "ADC.h"

void blink(void *par);

void vBMP280(void *par);


void systemReset()
{
	NVIC_SystemReset();
}

int main(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	GPIOC->CRH |= GPIO_CRH_MODE13;
	GPIOC->CRH &= ~GPIO_CRH_CNF13;

	UART_Init();

	UART_TransmitString("\033[2J");
	UART_TransmitString("Start\n\r");







	ADC_setWatchdogHandler(systemReset);




















	/*

	//Изменение размера дефолтной страници
	AT45DB641E_init();
	AT45DB641E_configurePageSize(AT45DB641E_eBinarysize);

	//форматирование фдешки
	// перед форматированием флешки установить FF_FS_NORTC и FF_USE_MKFS в еденичку
	uint8_t bufer[512];
	response = f_mkfs("",0,bufer,512);

	 */

/*

	uint8_t bufer[512];



	FATFS  FatSyastem; 	//обект FS
	UINT bw;
	FIL file;
	FRESULT response;

	response = f_mount(&FatSyastem, "", 0);

	response  = f_open(&file, "hello13.txt", FA_CREATE_NEW | FA_WRITE);
	f_write(&file, "Hello, world!\r\n", 25, &bw);
	f_close(&file);

	response  = f_open(&file, "hello13.txt", FA_READ);

	f_read(&file, bufer,256, &bw);

	f_close(&file);
	UART_TransmitDataViaDMA(bufer,bw);


*/







	xTaskCreate(blink,(char*)"blinkTask",500,NULL,tskIDLE_PRIORITY+1,NULL);
	//xTaskCreate(vBMP280,(char*)"BMP280Task",500,NULL,tskIDLE_PRIORITY+1,NULL);
	vTaskStartScheduler();




	while(1)
	{


	}
	return 0;
}

void blink(void *par)
{
	static char buf[100];

	vTaskDelay(5000);
	ADC_init(0xfff,2400);
	while(1)
	{
		GPIOC->BSRR=GPIO_BSRR_BR13;
		vTaskDelay(500);
		GPIOC->BSRR=GPIO_BSRR_BS13;
		vTaskDelay(500);

		sprintf(buf, "ADCV = %d, Uadc_c0 = %f \n\r" , ADC_getData(),ADC_getData()*(3.2/4096));
		//sprintf(buf,"IJ1=0x%x IJ2=0x%x IJ3=0x%x IJ4=0x%x \n\r",ADC1->JDR1,ADC1->JDR2, ADC1->JDR3 ,ADC1->JDR4);
		UART_TransmitString(buf);

	}

}

void vBMP280(void *par)
{
	static char array[100];
	double temp=0, press=0;

	BMP280_init();
	BMP280_setings(	eBMP280_setings_osrs_p16 |
			eBMP280_setings_osrs_tX2 |
			eBMP280_setings_filterX16 |
			eBMP280_setings_t_sb0_5 |
			eBMP280_setings_modeNormal
	);



	while(1)
	{

		temp = BMP280_getTemp();
		press = BMP280_getPress();




		UART_TransmitString("\033[2J");
		UART_TransmitString("Start\n\r");

		sprintf (array, "Temp = %lf \n\r", temp);
		UART_TransmitString(array);

		sprintf (array, "Pressure = %lf", press);
		UART_TransmitString(array);

		vTaskDelay(500);

	}
}



/*
 *
 	response = f_mount(&FatSyastem,"",1);
	if(response)
	{
#ifdef DEBUG_UART_TERMINAL
		sprintf(bufer,"1 Error to MOUNT cod is 0x%x\n\r",response);

		UART_TransmitString(bufer);


	}





	response = f_mkfs("",0,bufer,512);
	if(response)
	{
#ifdef DEBUG_UART_TERMINAL

		sprintf(bufer,"Error to Create cod is 0x%x\r",response);
		UART_TransmitString(bufer);
#endif

	}

#ifdef DEBUG_UART_TERMINAL

	sprintf(bufer,"1 Error to Create cod is 0x%x\n\r",response);
	UART_TransmitString(bufer);
#endif



	response = f_mount(&FatSyastem,"",1);
	if(response)
	{

		#ifdef DEBUG_UART_TERMINAL
			sprintf(bufer,"1 Error to MOUNT cod is 0x%x\n\r",response);

			UART_TransmitString(bufer);
		#endif
	}

 * */








