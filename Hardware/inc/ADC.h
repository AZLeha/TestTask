#ifndef INC_ADC_H_
#define INC_ADC_H_
#include <stdint.h>

typedef void (*ADC_Colback)(void);

void ADC_init(float fVoltageReference);
uint16_t ADC_getData(void);
float ADC_getVoltage(void);

void ADC_setWatchdogHandler(ADC_Colback xHandler);
void ADC_runWatchdog(uint16_t usHThreshold, uint16_t usLThreshold);


#endif
