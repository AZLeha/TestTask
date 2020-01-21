#ifndef INC_AT45DB641E_C_
#define INC_AT45DB641E_C_
#include <stdint.h>
#include <stdbool.h>

typedef enum{
	AT45DB641E_eBinarysize = 256,
	AT45DB641E_eDefaultSize = 264

}AT45DB641E_ePageSize;





#define SECTOR_SIZE AT45DB641E_eBinarysize

#define UART_DEBUG



/*!*
 *
 * 	\warning The nonvolatile register has a limit of 10,000 erase/program cycles; therefore, care should be taken to not switch between the size options more than 10,000 times
 * */
void AT45DB641E_configurePageSize(AT45DB641E_ePageSize xPageSize);


void AT45DB641E_init(void);


void AT45DB641E_readSector(void * pvInputData, uint16_t usSectorNumber);
void AT45DB641E_writeSector(void * pvOutputData, uint16_t usSectorNumber);


uint16_t AT45DB641E_status(void);


#endif /* INC_AT45DB641E_C_ */
