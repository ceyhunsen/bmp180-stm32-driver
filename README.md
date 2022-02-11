# Introduction

STM32 driver for BMP180 barometric pressure/temperature/altitude sensor. This driver lets you measure temperature, pressure, altitude and set custom sea pressure for calculating altitude for your needs.

## Units

* Temperature: Celsius
* Pressure: Pascal
* Altitude: Meter

## Simple Usage

```c
// Define bmp180_t struct instance
bmp180_t bmp180 = {.oversampling_setting = standart};

// Wait till initialization is complete
while (bmp180_init(&hi2c1, &bmp180));

// Get all the values
bmp180_get_all(&bmp180);
float temperature = bmp180.temperature;
int32_t pressure = bmp180.pressure;
float altitude = bmp180.altitude;
```

# Documentation

Documentation is available [online](https://bmp180-stm32-driver.readthedocs.io/en/latest/) and it includes information for:

* Adding to your project
* Units
* Usage
* Hardware oversampling
* Sea pressure
* API reference

# License

[MIT License](LICENSE)
