/**
 * \defgroup terminal
 *
 * \brief Командный интерпретатор
 * \addtogroup terminal
 *  @{
 */
#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"

//! Размер буфера команд
#define SIZE_RX_BUFFER 100

//! Структура тела команды
struct Terminal_stateItem;

//! handle для работы с командой
typedef struct Terminal_stateItem * Terminal_stateHandle;


//! прототип для колбека, в котором производится обработка данных
//! \private
typedef bool (*Terminal_Colback)(Terminal_stateHandle this, void *);


/// Функция ожидает ввод команды с клавиатуры
/*!
 * \param[in] xTicksToWait Время ожидания получения команды
 * \return Вернёт True, когда в терминале будет нажат Enter.
 * Если время ожидания истекло, то вернёт false.
 */
bool vGetComand(TickType_t xTicksToWait);


/// Функция для возврата на 1 уровень вниз по списку
/*!
 * \param[in] thisState handle текущего уровня
 */
void Terminal_exitStae(Terminal_stateHandle thisState);


/// Функция выводит список вложеных команд
/*!
 * Вывод состоит из названия самой команды и её описания
 *
 * \param[in] pxRootItem handle текущего уровня
 */
void Terminal_printInternalItemComandOrDescription(Terminal_stateHandle pxRootItem);


/// Функция создаёт объект команды
/*!
 *
 * \param[in] pcDescription Описание команды может
 * \param[in] pcComand handle Сама команда
 * \param[in] pcMessage handle Сообщение после выполнения команды
 * \param[in] xHandler handle Функция обратного вызова, которая будет вызвана при вызове команды
 */
Terminal_stateHandle Terminal_CrateState(char *pcDescription, char *pcComand, char *pcMessage, uint8_t ucStatesCount, Terminal_Colback xHandler);


/// Функция добавляет объект в список родителя
/*!
 * \param[in] xStateItem handle команды которые необходимо добавить в список
 * \param[in] pxParentItem handle команды родителя
 */
bool Terminal_addAreStateToList(Terminal_stateHandle xStateItem,Terminal_stateHandle pxParentItem);


/// Функция инициализации терминальной оболочки
void Terminal_Init(void);


/// Функция для прокрутки состояний терминальной оболочки
/*!
 * Осуществляется прокрутка одной команды, по завершению работы будет получен указатель на следующую
 * команду. Подразумевается что данная функция будет крутиться в бесконечном цикле.
 * \param[in] pxRootItem handle текущего уровня
 * \return Возвращает объект следующей итерации
 */
Terminal_stateHandle Terminal_stateMachine(Terminal_stateHandle pxRootItem);

//! }@
