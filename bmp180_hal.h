/*!
 * @author Ceyhun Sen
 * */

#ifndef __BMP180_H
#define __BMP180_H

#define HAL_LIB "stm32f4xx_hal.h"

#include <stdint.h>
#include HAL_LIB

typedef struct bmp180_t {
	float temperature;
	float pressure;
	float altitude;
	uint32_t base_pressure;

	// Callibration data
	int8_t   oss;
	int16_t  AC1;
	int16_t  AC2;
	int16_t  AC3;
	uint16_t AC4;
	uint16_t AC5;
	uint16_t AC6;
	int16_t  B1;
	int16_t  B2;
	int32_t  B3;
	uint32_t B4;
	int32_t  B5;
	int32_t  B6;
	uint32_t B7;
	int16_t  MB;
	int16_t  MC;
	int16_t  MD;
} bmp180_t;

uint8_t BMP180_init(bmp180_t *bmp180, uint8_t oss);
void BMP180_get_temperature(bmp180_t *bmp180);
void BMP180_get_pressure(bmp180_t *bmp180);

#endif
