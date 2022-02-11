/**
 * @file
 * @author Ceyhun Şen
 * */

#ifndef __BMP180_INTERNALS_H
#define __BMP180_INTERNALS_H

#include "bmp180.h"
#include <stdint.h>

// Slave address
#define BMP180_7_BIT_ADDRESS 0b1110111
#define BMP180_ADDRESS      (BMP180_7_BIT_ADDRESS << 1)

// Registers
#define CALIB     0xAA
#define ID        0xD0
#define SOFT      0xE0
#define CTRL_MEAS 0xF4
#define OUT_MSB   0xF6
#define OUT_LSB   0xF7
#define OUT_XLSB  0xF8

// Private defines
#define convert8bitto16bit(x, y)  (((x) << 8) | (y))
#define powerof2(x)                 (1 << (x))

// Private function prototypes
static int16_t _bmp180_read_ut(bmp180_t *bmp180);
static int32_t _bmp180_read_up(bmp180_t *bmp180);

#endif // __BMP180_INTERNALS_H
