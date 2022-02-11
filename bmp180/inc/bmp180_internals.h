/**
 * @file
 * @author Ceyhun Şen
 * */

#ifndef __BMP180_INTERNALS_H
#define __BMP180_INTERNALS_H

#include "bmp180.h"
#include <stdint.h>

/**
 * @def I2C_LIB
 * Library that includes I2C functions.
 * Change this definition to your MCU's I2C HAL library. E.g. "stm32f4xx_hal.h".
 * */
#define I2C_LIB "stm32f4xx_hal.h"

// Slave address
#define BMP180_7_BIT_ADDRESS         0b1110111
#define BMP180_ADDRESS              (BMP180_7_BIT_ADDRESS << 1)

// Registers
#define CALLIBRATION_COEF_REGISTERS  0xAA
#define CHIP_ID                      0xD0

// Private defines
#define convert8bitto16bit(x, y)  (((x) << 8) | (y))
#define powerof2(x)                 (1 << (x))

// Private function prototypes
static int16_t _BMP180_Read_ut(bmp180_t *bmp180);
static int32_t _BMP180_Read_up(bmp180_t *bmp180);

#endif // __BMP180_INTERNALS_H
