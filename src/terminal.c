#include "terminal.h"

#include <string.h>
#include "UART.h"
#include "FreeRTOS.h"
#include "queue.h"

typedef struct Terminal_stateItem
{
	char *pcDescription;
	char *pcActivatingCommand;
	char *pcMessage;
	uint8_t ucInternalStatesCount;
	Terminal_Colback xHandler;
	Terminal_stateHandle *xInternalStates;
	Terminal_stateHandle xPreviousState;
}xStateItem;




static void vRxHandler(void);




static char RXBuffer[SIZE_RX_BUFFER];



static QueueHandle_t xRXQueue = NULL;

void Terminal_exitStae(Terminal_stateHandle thisState)
{
	if(thisState->xPreviousState->xPreviousState != NULL)
	thisState->xPreviousState = thisState->xPreviousState->xPreviousState;
}


void Terminal_printInternalItemComandOrDescription(Terminal_stateHandle pxRootItem)
{
	pxRootItem = pxRootItem->xPreviousState;

	if(pxRootItem->ucInternalStatesCount != 0)
	{
		for(uint8_t i = 0; i < pxRootItem->ucInternalStatesCount; i++)
		{
			if(pxRootItem->xInternalStates[i] != NULL)
			{

				if(pxRootItem->xInternalStates[i]->pcActivatingCommand != NULL)
				{
					UART_TransmitString(pxRootItem->xInternalStates[i]->pcActivatingCommand);
					UART_TransmitString(" - ");
				}

				if(pxRootItem->xInternalStates[i]->pcActivatingCommand != NULL)
				{
					UART_TransmitString(pxRootItem->xInternalStates[i]->pcDescription);
					UART_TransmitString("\n\r");
				}
			}
			else break;
		}
	}
}


Terminal_stateHandle Terminal_CrateState(char *pcDescription, char *pcComand, char *pcMessage,
										uint8_t ucStatesCount, Terminal_Colback xHandler)
{
	Terminal_stateHandle localHandel = pvPortMalloc(sizeof(xStateItem));
	if(localHandel == NULL) return NULL;

	if(pcDescription != NULL)
	{
		localHandel->pcDescription = pvPortMalloc(strlen(pcDescription) * sizeof(char));
		if(localHandel->pcDescription == NULL) return NULL;
		strcpy (localHandel->pcDescription, pcDescription);
	}
	else localHandel->pcDescription = NULL;

	if(pcComand != NULL)
	{
		localHandel->pcActivatingCommand = pvPortMalloc(strlen(pcComand) * sizeof(char));
		if(localHandel->pcActivatingCommand == NULL) return NULL;
		strcpy (localHandel->pcActivatingCommand, pcComand);
	}
	else localHandel->pcActivatingCommand = NULL;

	if(pcMessage != NULL)
	{
		localHandel->pcMessage = pvPortMalloc(strlen(pcMessage) * sizeof(char));
		if(localHandel->pcMessage == NULL) return NULL;
		strcpy (localHandel->pcMessage, pcMessage);
	}
	else localHandel->pcMessage = NULL;

	if(ucStatesCount != 0)
	{
		localHandel->xInternalStates = pvPortMalloc(ucStatesCount * sizeof(void* ));
		if(localHandel->xInternalStates == NULL) return NULL;
		for(uint8_t i = 0; i < ucStatesCount; i++) localHandel->xInternalStates[i] = NULL;
	}
	else localHandel->xInternalStates = NULL;


	localHandel->xHandler = xHandler;
	localHandel->xPreviousState = NULL;
	localHandel->ucInternalStatesCount = ucStatesCount;

	return localHandel;

}


bool Terminal_addAreStateToList(Terminal_stateHandle xStateItem,Terminal_stateHandle pxParentItem)
{
	if(pxParentItem->ucInternalStatesCount != 0)
	{
		for(uint8_t i = 0; i < pxParentItem->ucInternalStatesCount; i++)
		{
			if(pxParentItem->xInternalStates[i] == NULL)
			{
				pxParentItem->xInternalStates[i] = xStateItem;
				xStateItem->xPreviousState = NULL;
				return true;
			}
		}
	}
	return false;
}



void Terminal_Init(void)
{
	UART_Init();

	UART_setIRQRxHandler(vRxHandler);

	xRXQueue = xQueueCreate(SIZE_RX_BUFFER / 2, sizeof(uint8_t));
	if(xRXQueue == NULL)
	{
		UART_TransmitString("Cannot create mutex \n\r");
		vTaskDelay(1000);
	}
	UART_TransmitString("\033[2J");

}


Terminal_stateHandle Terminal_stateMachine(Terminal_stateHandle pxRootItem)
{

	if(pxRootItem->xHandler != NULL)
	{
		if(pxRootItem->xHandler(pxRootItem, RXBuffer))
		{
			if(pxRootItem->xPreviousState != NULL)
			return pxRootItem->xPreviousState;
		}
	}


	if(pxRootItem->pcMessage != NULL)
	{
		UART_TransmitString(pxRootItem->pcMessage);
	}

	vGetComand(portMAX_DELAY);

	if(pxRootItem->ucInternalStatesCount != 0)
	{
		for(uint8_t i = 0; i < pxRootItem->ucInternalStatesCount; i++)
		{
			if(pxRootItem->xInternalStates[i] != NULL)
			{
				if(pxRootItem->xInternalStates[i]->pcActivatingCommand != NULL)
				{
					char *ptrOccurrence = strstr(RXBuffer, pxRootItem->xInternalStates[i]->pcActivatingCommand) ;

					if((ptrOccurrence != NULL) && (ptrOccurrence - RXBuffer == 0) )
					{
						UART_TransmitString("\n\r");
						pxRootItem->xInternalStates[i]->xPreviousState = pxRootItem;
						return pxRootItem->xInternalStates[i];
					}
				}
			}
			else break;
		}

		if(pxRootItem->pcDescription != NULL)
		{
			UART_TransmitString("\n\r");
			UART_TransmitString(pxRootItem->pcDescription);
			UART_TransmitString("\n\r");
		}

	}

	return pxRootItem;
}










static void vRxHandler(void)
{
	uint8_t data = UART_ReadByte();
	xQueueSendFromISR(xRXQueue, &data , NULL);
}

bool vGetComand(TickType_t xTicksToWait)
{
	static int16_t i=0;

	for(; i<SIZE_RX_BUFFER; i++)
	{
		if(xQueueReceive(xRXQueue, RXBuffer+i, xTicksToWait) == pdFALSE) break;

		//key press backspace
		if((RXBuffer[i] == 0x8) && (i>=0))
		{
			if(i==0) {i--; continue;}
			else i-=2;
			if(i>=-1) UART_TransmitString("\b \b");
			continue;
		}

		if(RXBuffer[i] == '\r')
		{
			RXBuffer[i] = '\0';
			i=0;
			return true;
		}
		UART_TransmitByte(RXBuffer[i]);
	}
	return false;
}

