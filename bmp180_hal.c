/**
 * @file
 * @author Ceyhun Şen
 * */

#include "bmp180_hal.h"
#include <math.h>

// Slave address
#define BMP180_7_BIT_ADDRESS         0b1110111
#define BMP180_ADDRESS              (BMP180_7_BIT_ADDRESS << 1)

// Registers
#define CALLIBRATION_COEF_REGISTERS  0xAA
#define CHIP_ID                      0xD0

// Private defines and functions
#define cnvrt8to16(x, y) (((x) << 8) | (y))
#define powerof2(x)      (1 << (x))

// Private function prototypes
static int16_t _BMP180_Read_ut(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);
static int32_t _BMP180_Read_up(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180);


/**
 * @brief Initialize sensor and get callibration values.
 * @returns 0 on success, 1 on sensor is not ready, 2 on sensor error.
 * @param hi2cx I2C handle.
 * @param bmp180 `bmp180_t` struct to initialize.
 * */
uint8_t BMP180_Init(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)
{
	// Check if device is ready
	if (HAL_I2C_IsDeviceReady(hi2cx, BMP180_ADDRESS, 1, HAL_MAX_DELAY) != HAL_OK)
		return 1;

	uint8_t buffer[22];

	// Check if device ID is correct
	HAL_I2C_Mem_Read(hi2cx, BMP180_ADDRESS, CHIP_ID, 1, buffer, 1, HAL_MAX_DELAY);
	if (buffer[0] != 0x55) {
		return 2;
	}

	HAL_I2C_Mem_Read(hi2cx, BMP180_ADDRESS, CALLIBRATION_COEF_REGISTERS, 1, buffer, 22, HAL_MAX_DELAY);

	// If any of the callibration data is 0x00 or 0xFF, the sensor is damaged
	for (uint8_t i = 0; i < 22; i += 2) {
		uint16_t combined_callibration_data = cnvrt8to16(buffer[i], buffer[i + 1]);
		if (combined_callibration_data == 0x00 || combined_callibration_data == 0XFF) {
			return 2;
		}
	}

	switch (bmp180->oversampling_setting) {
		case ultra_low_power:
			bmp180->oss = 0;
			break;
		case standart:
			bmp180->oss = 1;
			break;
		case high_resolution:
			bmp180->oss = 2;
			break;
		case ultra_high_resolution:
			bmp180->oss = 3;
			break;
		default:
			bmp180->oversampling_setting = standart;
			bmp180->oss = 1;
			break;
	}

	bmp180->AC1 = cnvrt8to16(buffer[0],  buffer[1]);
	bmp180->AC2 = cnvrt8to16(buffer[2],  buffer[3]);
	bmp180->AC3 = cnvrt8to16(buffer[4],  buffer[5]);
	bmp180->AC4 = cnvrt8to16(buffer[6],  buffer[7]);
	bmp180->AC5 = cnvrt8to16(buffer[8],  buffer[9]);
	bmp180->AC6 = cnvrt8to16(buffer[10], buffer[11]);
	bmp180->B1  = cnvrt8to16(buffer[12], buffer[13]);
	bmp180->B2  = cnvrt8to16(buffer[14], buffer[15]);
	bmp180->B3  = 0;
	bmp180->B4  = 0;
	bmp180->B5  = 0;
	bmp180->B6  = 0;
	bmp180->B7  = 0;
	bmp180->MB  = cnvrt8to16(buffer[16], buffer[17]);
	bmp180->MC  = cnvrt8to16(buffer[18], buffer[19]);
	bmp180->MD  = cnvrt8to16(buffer[20], buffer[21]);
	bmp180->sea_pressure = 101325;
	return 0;
}

/**
 * @brief Get all sensor data at once.
 * @param hi2cx I2C handle.
 * @param bmp180 `bmp180_t` struct to write data.
 * @retval None.
 * */
void BMP180_Get_All(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)
{
	BMP180_Get_Temperature(hi2cx, bmp180);
	BMP180_Get_Pressure(hi2cx, bmp180);
	BMP180_Get_Altitude(hi2cx, bmp180);
}

static int16_t _BMP180_Read_ut(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)
{
	uint8_t write_data = 0x2E, ut_data[2];

	HAL_I2C_Mem_Write(hi2cx, BMP180_ADDRESS, 0xF4, 1, &write_data, 1, HAL_MAX_DELAY);
	HAL_Delay(5);
	HAL_I2C_Mem_Read(hi2cx, BMP180_ADDRESS, 0xF6, 1, ut_data, 2, HAL_MAX_DELAY);
	return (cnvrt8to16(ut_data[0], ut_data[1]));
}

static int32_t _BMP180_Read_up(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)
{
	uint8_t write_data = 0x34 + (bmp180->oss << 6), up_data[3];
	HAL_I2C_Mem_Write(hi2cx, BMP180_ADDRESS, 0xF4, 1, &write_data, 1, HAL_MAX_DELAY);
	uint8_t wait = 0;
	switch (bmp180->oversampling_setting) {
		case ultra_low_power:
			wait = 5;
			break;
		case standart:
			wait = 8;
			break;
		case high_resolution:
			wait = 14;
			break;
		case ultra_high_resolution:
			wait = 26;
			break;
		default:
			wait = 5;
			break;
	}
	HAL_Delay(wait);
	HAL_I2C_Mem_Read(hi2cx, BMP180_ADDRESS, 0xF6, 1, up_data, 3, HAL_MAX_DELAY);
	return ((up_data[0] << 16) + (up_data[1] << 8) + up_data[2]) >> (8 - bmp180->oss);
}

/**
 * @brief Get temperature data as celsius.
 * @param hi2cx I2C handle.
 * @param bmp180 `bmp180_t` struct to write data.
 * @retval None.
 * */
void BMP180_Get_Temperature(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)
{
	int16_t ut = _BMP180_Read_ut(hi2cx, bmp180);
	int32_t X1, X2;

	X1 = (ut - bmp180->AC6) * bmp180->AC5 / powerof2(15);
	X2 = bmp180->MC * powerof2(11) / (X1 + bmp180->MD);
	bmp180->B5 = X1 + X2;
	bmp180->temperature = ((bmp180->B5 + 8) / powerof2(4)) / 10.0;
}

/**
 * @brief Get pressure data as pascal.
 * @param hi2cx I2C handle.
 * @param bmp180 `bmp180_t` struct to write data.
 * @retval None.
 * */
void BMP180_Get_Pressure(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)
{
	int32_t X1, X2, X3, up = _BMP180_Read_up(hi2cx, bmp180), p;
	bmp180->B6 = bmp180->B5 - 4000;
	X1 = (bmp180->B2 * (bmp180->B6 * bmp180->B6 / powerof2(12))) / powerof2(11);
	X2 = bmp180->AC2 * bmp180->B6 / powerof2(11);
	X3 = X1 + X2;
	bmp180->B3 = (((bmp180->AC1 * 4 + X3) << bmp180->oss) + 2) / 4;
	X1 = bmp180->AC3 * bmp180->B6 / powerof2(13);
	X2 = (bmp180->B1 * (bmp180->B6 * bmp180->B6 / powerof2(12))) / powerof2(16);
	X3 = ((X1 + X2) + 2) / powerof2(2);
	bmp180->B4 = bmp180->AC4 * (uint32_t)(X3 + 32768) / powerof2(15);
	bmp180->B7 = ((uint32_t)up - bmp180->B3) * (50000 >> bmp180->oss);
	if (bmp180->B7 < 0x80000000) {
		p = (bmp180->B7 * 2) / bmp180->B4;
	}
	else {
		p = (bmp180->B7 / bmp180->B4) * 2;
	}
	X1 = (p / powerof2(8)) * (p / powerof2(8));
	X1 = (X1 * 3038) / powerof2(16);
	X2 = (-7357 * p) / powerof2(16);
	p = p + (X1 + X2 + 3791) / powerof2(4);
	bmp180->pressure = p;
}

/**
 * @brief Get altitude data as meter.
 * @param hi2cx I2C handle.
 * @param bmp180 `bmp180_t` struct to write data.
 * @retval None.
 * */
void BMP180_Get_Altitude(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180)
{
	bmp180->altitude = 44330 * (1 - pow(((float)bmp180->pressure / (float)bmp180->sea_pressure), 1 / 5.255));
}

/**
 * @brief Set sea pressure as pascal.
 * @param hi2cx I2C handle.
 * @param bmp180 `bmp180_t` struct to write data.
 * @param sea_pressure New sea pressure.
 * @retval None.
 * */
void BMP180_Set_Sea_Pressure(I2C_HandleTypeDef *hi2cx, bmp180_t *bmp180, int32_t sea_pressure)
{
	bmp180->sea_pressure = sea_pressure;
}
