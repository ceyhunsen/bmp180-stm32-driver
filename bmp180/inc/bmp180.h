/**
 * @file
 * @author Ceyhun Şen
 * */

#ifndef __BMP180_H
#define __BMP180_H

/**
 * @def I2C_LIB
 * Library that includes I2C functions.
 * Change this definition to your MCU's I2C HAL library. E.g. "stm32f4xx_hal.h".
 * */
#define I2C_LIB "stm32f4xx_hal.h"

#include I2C_LIB
#include <stdint.h>

/**
 * @enum _bmp180_oversampling_settings
 * @brief Oversampling settings for BMP180 sensor.
 * */
enum _bmp180_oversampling_settings {
	ultra_low_power,
	standart,
	high_resolution,
	ultra_high_resolution
};

/**
 * @struct bmp180_t bmp180.h bmp180.h
 * @brief Holds sensor data, sensor settings and callibration values.
 * */
typedef struct bmp180_t {
	// I2C handle
	I2C_HandleTypeDef *hi2cx;

	// Sensor data
	float    temperature;
	int32_t  pressure;
	float    altitude;
	int32_t  sea_pressure;

	// Settings
	enum _bmp180_oversampling_settings oversampling_setting;
	uint8_t  oss;

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

uint8_t bmp180_init(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void bmp180_get_all(bmp180_t *bmp180);
void bmp180_get_temperature(bmp180_t *bmp180);
void bmp180_get_pressure(bmp180_t *bmp180);
void bmp180_get_altitude(bmp180_t *bmp180);
void bmp180_set_sea_pressure(bmp180_t *bmp180, int32_t sea_pressure);

#endif // BMP180_H
