# Introduction

This STM32 HAL library is for BMP180 barometric pressure/temperature/altitude sensor.

# How To Use

## Adding To Your Project

* Copy [bmp180_hal.c](bmp180_hal.c) into your source directory.
* Copy [bmp180_hal.h](bmp180_hal.h) into your include directory.
* Change `HAL_LIB` definition in [bmp180_hal.h](bmp180_hal.h) (line 8) to your MCU's HAL library.

## Usage In Code

One can use this library like this:

```c
// Define bmp180_t struct instance
bmp180_t bmp180;

// Wait till initialization is complete
while (BMP180_init(&hi2c1, &bmp180, 0));

// Get all the values
BMP180_get_all(&hi2c1, &bmp180, 0);
float temperature = bmp180.temperature;
float pressure = bmp180.pressure;
float altitude = bmp180.altitude;
```

See [Api Reference](#api-reference) for function and struct definitions.

## API Reference

* `typedef struct bmp180_t`

Struct that holds calculated and callibration values. Public members are: `temperature`, `pressure` and `altitude`.

* `uint8_t BMP180_init(bmp180_t *bmp180, uint8_t oss)`

Initialization function. Call this function before calling other functions to get callibration values. This function returns 0 on success, 1 on fail.

* `void BMP180_get_all(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)`

Calculates temperature, pressure and altitude. Writes values to parameter `bmp180` pointer.

* `void BMP180_get_temperature(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)`

Calculates temperature. Writes values to parameter `bmp180` pointer.

* `void BMP180_get_pressure(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)`

Calculates pressure. Writes value to parameter `bmp180` pointer.

* `void BMP180_get_altitude(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)`

Calculates altitude. Writes value to parameter `bmp180` pointer.

* `void BMP180_set_sea_pressure(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180, uint32_t sea_pressure)`

Sets sea pressure. Changes `sea_pressure` member of the given `bmp180_t` pointer parameter. Default `sea_pressure` value is 101325;

# License

[MIT License](LICENSE)