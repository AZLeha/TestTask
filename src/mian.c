
#include "inc/tz.h"

#include "FreeRTOS.h"
#include "task.h"

int main(void)
{


	TZ_init();


	ADC_setWatchdogHandler(systemReset);

	//Создание задач и и запуск планировщика
	xTaskCreate(vBlink,(char*)"blinkTask",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+1,NULL);

	xTaskCreate(vTerminal,(char*)"terminalTask",500,NULL,tskIDLE_PRIORITY+2,NULL);



	vTaskStartScheduler();

	while(1)
	{
	}
	return 0;
}
