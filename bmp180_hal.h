/**
 * @file bmp180_hal.h
 * @author Ceyhun Şen
 * */

#ifndef __BMP180_H
#define __BMP180_H

/**
 * @def HAL_LIB
 * HAL library.
 * Change this definition to your MCU's HAL library. E.g. "stm32f4xx_hal.h".
 * */
#define HAL_LIB "stm32f4xx_hal.h"

#include HAL_LIB
#include <stdint.h>

/**
 * @enum _BMP180_oversampling_settings bmp180_hal.h bmp180_hal.h
 * @brief Oversampling settings for BMP180 sensor.
 * */
enum _BMP180_oversampling_settings {
	ultra_low_power,
	standart,
	high_resolution,
	ultra_high_resolution
};

/**
 * @struct bmp180_t bmp180_hal.h bmp180_hal.h
 * @brief Holds sensor data, sensor settings and callibration values.
 * */
typedef struct bmp180_t {
	// Sensor data
	float temperature;
	float pressure;
	float altitude;
	float sea_pressure;

	// Settings
	enum _BMP180_oversampling_settings oversampling_setting;
	uint8_t   oss;

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

uint8_t BMP180_init(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void BMP180_get_all(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void BMP180_get_temperature(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void BMP180_get_pressure(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void BMP180_get_altitude(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
void BMP180_set_sea_pressure(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180, float sea_pressure);

#endif
