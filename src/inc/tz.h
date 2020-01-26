
#ifndef INC_TZ_H_
#define INC_TZ_H_

#include <stdio.h>






/// Инициализация всего необходимого
void TZ_init(void);

/// Функция сброса, которая будет вызываться при событии сброса
void systemReset(void);




void vBlink(void *par);

/// Задача осуществляющая логирование
void vLogger(void *par);

/// командный интерфейс
void vTerminal(void *par);









#endif /* INC_TZ_H_ */
