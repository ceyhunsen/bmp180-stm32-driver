/**
 * @author Ceyhun Sen
 * */

#ifndef __BMP180_H
#define __BMP180_H

#define HAL_LIB "stm32f4xx_hal.h"

#include HAL_LIB
#include <stdint.h>

typedef struct bmp180_t {
	float temperature;
	float pressure;
	float altitude;
	uint32_t sea_pressure;

	// Settings
	int8_t   oss;

	// Callibration data
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

uint8_t BMP180_init(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180, uint8_t oss);
void BMP180_get_all(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void BMP180_get_temperature(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void BMP180_get_pressure(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void BMP180_get_altitude(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void BMP180_set_sea_pressure(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180, uint32_t sea_pressure);

#endif
