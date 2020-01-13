#include "inc/BMP280.h"
#include "I2C.h"

typedef enum
{
	BMP280_eDig_T1 = 0x88,
	BMP280_eDig_T2 = 0x8A,
	BMP280_eDig_T3 = 0x8C,
	BMP280_eDig_P1 = 0x8E,
	BMP280_eDig_P2 = 0x90,
	BMP280_eDig_P3 = 0x92,
	BMP280_eDig_P4 = 0x94,
	BMP280_eDig_P5 = 0x96,
	BMP280_eDig_P6 = 0x98,
	BMP280_eDig_P7 = 0x9A,
	BMP280_eDig_P8 = 0x9C,
	BMP280_eDig_P9 = 0x9E

}BMP280_MemoryMap_16bit;

typedef enum
{
	BMP280_eDeviceID =		0xD0,
	BMP280_eDeviceReset = 	0xE0,
	BMP280_eDeviceStatus =	0xF3,
	BMP280_eControl =		0xF4,
	BMP280_eConfig =		0xF5,
}BMP280_MemoryMap_8bit;

typedef enum
{
	BMP280_ePressure =	0xF7,
	BMP280_eTemp =		0xFA
}BMP280_MemoryMap_24bit;



typedef struct
{
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;

}CalibrationTabel;



CalibrationTabel calibrationTabel={0};



void BMP280_setings(eBMP280_setings xSetings)
{
	uint8_t data;

	data = xSetings;
	I2C_WriteData(BMP280_I2C_ADDRESS,BMP280_eControl,&data,1, eI2C_MSBFirst);

	data = xSetings>>8;
	I2C_WriteData(BMP280_I2C_ADDRESS,BMP280_eConfig,&data,1, eI2C_MSBFirst);

}

void BMP280_updateCalibrationTabel()
{
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_T1,&calibrationTabel.dig_T1, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_T2,&calibrationTabel.dig_T2, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_T3,&calibrationTabel.dig_T3, 2, eI2C_MSBLast);


	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_P1,&calibrationTabel.dig_P1, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_P2,&calibrationTabel.dig_P2, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_P3,&calibrationTabel.dig_P3, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_P4,&calibrationTabel.dig_P4, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_P5,&calibrationTabel.dig_P5, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_P6,&calibrationTabel.dig_P6, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_P7,&calibrationTabel.dig_P7, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_P8,&calibrationTabel.dig_P8, 2, eI2C_MSBLast);
	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eDig_P9,&calibrationTabel.dig_P9, 2, eI2C_MSBLast);

}



void BMP280_init(void)
{
	I2C_Init();


	//BMP280_setings();
	BMP280_updateCalibrationTabel();

	for(uint32_t i=0; i<0xFffff;i++) asm("NOP");
}

static int32_t t_fine = 0;

double BMP280_getTemp(void)
{


	int32_t adc_T=0;


	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_eTemp,&adc_T, 3, eI2C_MSBFirst);

	adc_T >>=4;

	double var1, var2, T;
	var1 = (((double)adc_T)/16384.0 - ((double)calibrationTabel.dig_T1)/1024.0) * ((double)calibrationTabel.dig_T2);

	var2 = ((((double)adc_T)/131072.0 - ((double)calibrationTabel.dig_T2)/8192.0) *
			(((double)adc_T)/131072.0 - ((double) calibrationTabel.dig_T1)/8192.0)) * ((double)calibrationTabel.dig_T3);

	t_fine = (int32_t)(var1 + var2);

	T = (var1 + var2) / 5120.0;
	return T;

}
double BMP280_getPress(void)
{

	int32_t adc_P=0;
	BMP280_getTemp();

	I2C_ReadData(BMP280_I2C_ADDRESS,BMP280_ePressure,&adc_P, 3, eI2C_MSBFirst);
	adc_P >>=4;



	double var1, var2, p;
	var1 = ((double)t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)calibrationTabel.dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)calibrationTabel.dig_P5) * 2.0;
	var2 = (var2/4.0)+(((double)calibrationTabel.dig_P4) * 65536.0);
	var1 = (((double)calibrationTabel.dig_P3) * var1 * var1 / 524288.0 + ((double)calibrationTabel.dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)calibrationTabel.dig_P1);
	if (var1 == 0.0)
	{
		return 0; // avoid exception caused by division by zero
	}

	p = 1048576.0 - (double)adc_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)calibrationTabel.dig_P9) * p * p / 2147483648.0;
	var2 = p * ((double)calibrationTabel.dig_P8) / 32768.0;
	p = p + (var1 + var2 + ((double)calibrationTabel.dig_P7)) / 16.0;
	return p;

}
