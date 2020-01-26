#include "stm32f10x.h"
#include <string.h>
#include "inc/tz.h"

#include "FreeRTOS.h"
#include "task.h"


#include "terminal.h"
#include "UART.h"

#include "BMP280.h"

#include "ff.h"
#include "fff.h"
#include "ADC.h"




static FATFS  FatSyastem;


void systemReset()
{
	NVIC_SystemReset();
}

void TZ_init(void)
{
	ADC_setWatchdogHandler(systemReset);

	BMP280_init();
	BMP280_setings(eBMP280_setings_osrs_p16 |
				   eBMP280_setings_osrs_tX2 |
				   eBMP280_setings_filterX16 |
				   eBMP280_setings_t_sb0_5 |
				   eBMP280_setings_modeNormal);


	//Здесь можно задать пределы на watchdog, но в попугаях
	ADC_init(3.2);
	ADC_runWatchdog(0xfff,2400);
	ADC_setWatchdogHandler(systemReset);


	f_mount(&FatSyastem, "", 0);
	ff_init();
}






void vBlink(void *par)
{

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	GPIOC->CRH |= GPIO_CRH_MODE13;
	GPIOC->CRH &= ~GPIO_CRH_CNF13;

	while(1)
	{
		GPIOC->BSRR=GPIO_BSRR_BR13;
		vTaskDelay(500);
		GPIOC->BSRR=GPIO_BSRR_BS13;
		vTaskDelay(500);
	}
}





void vLogger(void *par)
{

	static char logBuffer[100];
	static char filename[100];

	strcpy(filename, par);
	int iteration = 0;

	while(1)
	{
		FIL fil;
		ff_open(&fil, filename ,FA_OPEN_APPEND |FA_WRITE);

			sprintf(logBuffer,"%i.log t = %f p = %f adc = %f \n",iteration, BMP280_getTemp(), BMP280_getPress(), ADC_getVoltage());
			f_puts(logBuffer, &fil);

		ff_close(&fil);

		vTaskDelay(10000);
		iteration++;

	}
}




char * faindProperty(char * ptr)
{
	uint16_t i ;
	char *dataPtr = ptr;
	for(i = 0; (i < SIZE_RX_BUFFER) && (dataPtr[i] != ' '); i++) asm("NOP");
	for(; (i < SIZE_RX_BUFFER) && (dataPtr[i] == ' '); i++) asm("NOP");

	return dataPtr+i;
}



bool logComand(Terminal_stateHandle this, void * ptr)
{

	xTaskCreate(vLogger,(char*)"logerTask",500,faindProperty(ptr),tskIDLE_PRIORITY+2,NULL);
	return true;
}


bool lsComand(Terminal_stateHandle this, void * ptr)
{

	FRESULT res;
	static FILINFO fno;

	ff_getcwd(ptr,100);

	DIR dir;
	res = ff_opendir(&dir, ptr);                       /* Open the directory */
	if (res == FR_OK) {
		for (;1;) {
			res = ff_readdir(&dir, &fno);                   /* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */


			if (fno.fattrib & AM_DIR)
				 UART_TransmitString("/");
			UART_TransmitString(fno.fname);
			UART_TransmitString("\n\r");                                  /* It is a file. */
				//printf("%s/%s\n", path, fno.fname);

		}
		ff_closedir(&dir);
	}

	return true;
}







bool touchhComand(Terminal_stateHandle this, void * ptr)
{


	FIL fil;
	ff_open(&fil, faindProperty(ptr), FA_CREATE_NEW);
	ff_close(&fil);

	return true;
}


bool mkdirComand(Terminal_stateHandle this, void * ptr)
{

	ff_mkdir(faindProperty(ptr));
	return true;
}


bool cdComand(Terminal_stateHandle this, void * ptr)
{
	ff_chdir(faindProperty(ptr));
	return true;
}


bool rmComand(Terminal_stateHandle this, void * ptr)
{

	ff_unlink(faindProperty(ptr));
	return true;
}


bool pwdComand(Terminal_stateHandle this, void * ptr)
{

	ff_getcwd(ptr,100);
	UART_TransmitString(ptr);
	UART_TransmitString("\n\r");
	return true;
}


bool printComand(Terminal_stateHandle this, void * ptr)
{
	FIL fil;

	ff_open(&fil, faindProperty(ptr), FA_READ);


	for(uint16_t i=0; i<1000; i++)
	{
		if(ff_gets(ptr,100, &fil) == NULL) break;
		UART_TransmitString(ptr);
		UART_TransmitString("\r");
	}
	ff_close(&fil);


	UART_TransmitString("\n\r");
	return true;
}


bool help(Terminal_stateHandle this, void * ptr)
{

	Terminal_printInternalItemComandOrDescription(this);
	return true;
}


bool clear(Terminal_stateHandle this, void * ptr)
{
	UART_TransmitString("\033[2J");
	return true;
}







bool Comand_bmp280(Terminal_stateHandle this, void * ptr)
{
	//char *ptrOccurrence = strstr(ptr, "-t") ;
	char *inputSrting = ptr;


	char flags[3]={0};
	bool flag = true;
	//bmp280 6
	for(uint16_t i = 6; i < SIZE_RX_BUFFER; i++)
	{
		if(inputSrting[i] == '\r' || inputSrting[i] == '\0') break;

		if(inputSrting[i] == '-')
		{
			i++;
			switch (inputSrting[i])
			{
				case 't':
					flags[0]='t';
				break;

				case 'p':
					flags[1]='p';
				break;

				case 'w':
					flags[2]='w';
				break;
				default:
					flag = false;
				break;
			}

		}
		else if(inputSrting[i] == ' ') continue;

		else
		{
			flag = false;
			break;
		}

	}


	if(!flag)
	{
		UART_TransmitString("Command can accept only the following flags: \
										\n\r-t to the get temperatures\
										\n\r-p to the get pressure\
										\n\r-w to the get height metres above sea level\n\r");
		return true;
	}


	if(flags[0] == 't')
	{
		sprintf(ptr,"Temperature = %f C \n\r", BMP280_getTemp());
		UART_TransmitString(ptr);
	}

	if(flags[1] == 'p')
	{
		sprintf(ptr,"Pressure = %f pa \n\r", BMP280_getPress());
		UART_TransmitString(ptr);
	}

	if(flags[2] == 'w')
	{
		//Здесь могла быть формула для расчёта высоты, но мне лень)
	}

	return true;
}


bool exitBmp(Terminal_stateHandle this, void * ptr)
{

	Terminal_exitStae(this);
	return true;
}




void vTerminal(void *par)
{
	//Канал для связи с PC (Заточен под minicom)

	Terminal_Init();


	Terminal_stateHandle rootItem = Terminal_CrateState("Command not found please enter valid command or entry command \"help\"", NULL, "Please enter command >> ", 11, NULL);




	Terminal_stateHandle bmp280Item = Terminal_CrateState("Software test case", "bmp280", "Pleas entry BMP command >> ", 0, Comand_bmp280);
	Terminal_stateHandle logItem = Terminal_CrateState("It is command to be running logger. To work, you must specify a file name", "log", NULL, 0, logComand);



	Terminal_stateHandle lsItem = Terminal_CrateState("List directory contents", "ls", NULL, 0, lsComand);
	Terminal_stateHandle mkdirItem = Terminal_CrateState("Make directory", "mkdir", NULL, 0, mkdirComand);
	Terminal_stateHandle touchItem = Terminal_CrateState("Create file", "touch", NULL, 0, touchhComand);


	Terminal_stateHandle cdItem = Terminal_CrateState("Change the shell working directory", "cd", NULL, 0, cdComand);
	Terminal_stateHandle rmItem = Terminal_CrateState("Remove files or directories", "rm", NULL, 0, rmComand);

	Terminal_stateHandle pwdItem = Terminal_CrateState("Present working directory", "pwd", NULL, 0, pwdComand);
	Terminal_stateHandle printItem = Terminal_CrateState("Print file", "print", NULL, 0, printComand);

	Terminal_stateHandle helpItem = Terminal_CrateState("Command help", "help", NULL, 0, help);
	Terminal_stateHandle clsItem = Terminal_CrateState("Clear scale", "cls", NULL, 0, clear);




	Terminal_addAreStateToList(bmp280Item, rootItem);
	Terminal_addAreStateToList(logItem, rootItem);
	Terminal_addAreStateToList(lsItem, rootItem);
	Terminal_addAreStateToList(mkdirItem, rootItem);
	Terminal_addAreStateToList(touchItem, rootItem);
	Terminal_addAreStateToList(cdItem, rootItem);
	Terminal_addAreStateToList(rmItem, rootItem);
	Terminal_addAreStateToList(pwdItem, rootItem);
	Terminal_addAreStateToList(printItem, rootItem);
	Terminal_addAreStateToList(helpItem, rootItem);
	Terminal_addAreStateToList(clsItem, rootItem);



	Terminal_stateHandle workItem = rootItem;
	while(1)
	{
		workItem = Terminal_stateMachine(workItem);

		//vTaskDelay(1000);

	}
}
