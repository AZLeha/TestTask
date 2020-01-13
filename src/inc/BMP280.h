#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include <stdint.h>


#define BMP280_I2C_ADDRESS 0x76


typedef enum
{

#define BMP280_SHIFT_T_SD 13
#define BMP280_SHIFT_FILTER 10
#define BMP280_SHIFT_SPI3W_EN 8
#define BMP280_SHIFT_OSRS_T 5
#define BMP280_SHIFT_OSRS_P 2
#define BMP280_SHIFT_MODE 0



	eBMP280_setings_t_sb0_5 = 	0x00,
	eBMP280_setings_t_sb62_5 = 	0x01 << BMP280_SHIFT_T_SD,
	eBMP280_setings_t_sb125 = 	0x02 << BMP280_SHIFT_T_SD,
	eBMP280_setings_t_sb250 = 	0x03 << BMP280_SHIFT_T_SD,
	eBMP280_setings_t_sb500 = 	0x04 << BMP280_SHIFT_T_SD,
	eBMP280_setings_t_sb1000 = 	0x05 << BMP280_SHIFT_T_SD,
	eBMP280_setings_t_sb2000 = 	0x06 << BMP280_SHIFT_T_SD,
	eBMP280_setings_t_sb4000 = 	0x07 << BMP280_SHIFT_T_SD,

	eBMP280_setings_filterOff = 0,
	eBMP280_setings_filterX2 = 	0x01 << BMP280_SHIFT_FILTER,
	eBMP280_setings_filterX4 = 	0x02 << BMP280_SHIFT_FILTER,
	eBMP280_setings_filterX8 = 	0x03 << BMP280_SHIFT_FILTER,
	eBMP280_setings_filterX16 = 0x04 << BMP280_SHIFT_FILTER,

	eBMP280_setings_spi3w_en = 0x1 << BMP280_SHIFT_SPI3W_EN,

	eBMP280_setings_osrs_tSkipped = 0,
	eBMP280_setings_osrs_tX1 = 		0x01 << BMP280_SHIFT_OSRS_T,
	eBMP280_setings_osrs_tX2 = 		0x02 << BMP280_SHIFT_OSRS_T,
	eBMP280_setings_osrs_tX4 = 		0x03 << BMP280_SHIFT_OSRS_T,
	eBMP280_setings_osrs_tx8 = 		0x04 << BMP280_SHIFT_OSRS_T,
	eBMP280_setings_osrs_t16 = 		0x05 << BMP280_SHIFT_OSRS_T,

	eBMP280_setings_osrs_pSkipped = 0,
	eBMP280_setings_osrs_pX1 = 		0x01 << BMP280_SHIFT_OSRS_P,
	eBMP280_setings_osrs_pX2 = 		0x02 << BMP280_SHIFT_OSRS_P,
	eBMP280_setings_osrs_pX4 = 		0x03 << BMP280_SHIFT_OSRS_P,
	eBMP280_setings_osrs_px8 = 		0x04 << BMP280_SHIFT_OSRS_P,
	eBMP280_setings_osrs_p16 = 		0x05 << BMP280_SHIFT_OSRS_P,

	eBMP280_setings_modeSleep =		0,
	eBMP280_setings_modeForced = 	0x01 << BMP280_SHIFT_MODE,
	eBMP280_setings_modeNormal =	0x03 << BMP280_SHIFT_MODE


#undef BMP280_SHIFT_T_SD
#undef BMP280_SHIFT_FILTER
#undef BMP280_SHIFT_SPI3W_EN
#undef BMP280_SHIFT_OSRS_T
#undef BMP280_SHIFT_OSRS_P
#undef BMP280_SHIFT_MODE
}eBMP280_setings;


void BMP280_setings(eBMP280_setings xSetings);
void BMP280_init(void);

double BMP280_getTemp(void);
double BMP280_getPress(void);


#endif /* INC_BMP280_H_ */
