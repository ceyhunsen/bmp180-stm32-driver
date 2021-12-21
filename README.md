# Introduction

This STM32 library is for BMP180 barometric pressure/temperature/altitude sensor.

# How To Use

## Adding To Your Project

* Copy [bmp180_hal.c](bmp180_hal.c) into your source directory.
* Copy [bmp180_hal.h](bmp180_hal.h) into your include directory.
* Change `I2C_HANDLE` definition in [bmp180_hal.c](bmp180_hal.c) (line 8) to your own handle of the I2C protocol.
* Change `HAL_LIB` definition in [bmp180_hal.h](bmp180_hal.h) (line 8) to your own HAL library.