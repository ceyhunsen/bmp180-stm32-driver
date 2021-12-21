/**
 * @author Ceyhun Sen
 * */

#include "bmp180_hal.h"
#include <math.h>

#define I2C_HANDLE        hi2c1
extern  I2C_HandleTypeDef I2C_HANDLE;
#define BMP180_I2C       &I2C_HANDLE

#define BMP180_BIN_ADDRESS    (0b1110111 << 1)
#define BMP180_WRITE_ADDRESS   BMP180_BIN_ADDRESS         // 0xEE
#define BMP180_READ_ADDRESS   (BMP180_WRITE_ADDRESS + 1)  // 0xEF
#define BMP180_REGISTER_START  0xAA

#define cnvrt8to16(x, y) (((x) << 8) | (y))
#define powerof2(x)      (1 << (x))

static int32_t BMP180_read_ut(bmp180_t *bmp180);
static int32_t BMP180_read_up(bmp180_t *bmp180);


/**
 * @brief Initialize callibration values
 * @retval 0 on success, 1 on fail
 * */
uint8_t BMP180_init(bmp180_t *bmp180, uint8_t oss)
{
	uint8_t callibration_data[22];
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_READ_ADDRESS, BMP180_REGISTER_START, 1, callibration_data, 22, HAL_MAX_DELAY);

	for (uint8_t i = 0; i < 22; i += 2) {
		uint16_t combined_callibration_data = cnvrt8to16(callibration_data[i], callibration_data[i + 1]);
		if (combined_callibration_data == 0 || combined_callibration_data == 255) {
			return 1;
		}
	}

	bmp180->oss = oss;
	bmp180->AC1 = cnvrt8to16(callibration_data[0],  callibration_data[1]);
	bmp180->AC2 = cnvrt8to16(callibration_data[2],  callibration_data[3]);
	bmp180->AC3 = cnvrt8to16(callibration_data[4],  callibration_data[5]);
	bmp180->AC4 = cnvrt8to16(callibration_data[6],  callibration_data[7]);
	bmp180->AC5 = cnvrt8to16(callibration_data[8],  callibration_data[9]);
	bmp180->AC6 = cnvrt8to16(callibration_data[10], callibration_data[11]);
	bmp180->B1  = cnvrt8to16(callibration_data[12], callibration_data[13]);
	bmp180->B2  = cnvrt8to16(callibration_data[14], callibration_data[15]);
	bmp180->B3  = 0;
	bmp180->B4  = 0;
	bmp180->B5  = 0;
	bmp180->B6  = 0;
	bmp180->B7  = 0;
	bmp180->MB  = cnvrt8to16(callibration_data[16], callibration_data[17]);
	bmp180->MC  = cnvrt8to16(callibration_data[18], callibration_data[19]);
	bmp180->MD  = cnvrt8to16(callibration_data[20], callibration_data[21]);
	bmp180->sea_pressure = 101325;
	return 0;
}

void BMP180_get_all(bmp180_t *bmp180)
{
	BMP180_get_temperature(bmp180);
	BMP180_get_pressure(bmp180);
	BMP180_get_altitude(bmp180);
}

static int32_t BMP180_read_ut(bmp180_t *bmp180)
{
	uint8_t write_data = 0x2E, ut_data[2];

	HAL_I2C_Mem_Write(BMP180_I2C, BMP180_WRITE_ADDRESS, 0xF4, 1, &write_data, 1, HAL_MAX_DELAY);
	HAL_Delay(5);
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_READ_ADDRESS, 0xF6, 1, ut_data, 2, HAL_MAX_DELAY);
	return (cnvrt8to16(ut_data[0], ut_data[1]));
}

void BMP180_get_temperature(bmp180_t *bmp180)
{
	int32_t ut = BMP180_read_ut(bmp180);
	int32_t X1, X2;

	X1 = (ut - bmp180->AC6) * bmp180->AC5 / powerof2(15);
	X2 = bmp180->MC * powerof2(11) / (X1 + bmp180->MD);
	bmp180->B5 = X1 + X2;
	bmp180->temperature = ((bmp180->B5 + 8) / powerof2(4)) / 10.0;
}

static int32_t BMP180_read_up(bmp180_t *bmp180)
{
	uint8_t write_data = 0x34 + ((bmp180->oss << 6)), up_data[3];
	HAL_I2C_Mem_Write(BMP180_I2C, BMP180_WRITE_ADDRESS, 0xF4, 1, &write_data, 1, HAL_MAX_DELAY);
	uint8_t wait = 0;
	switch (bmp180->oss) {
		case 0:
			wait = 5;
			break;
		case 1:
			wait = 8;
			break;
		case 2:
			wait = 14;
			break;
		case 3:
			wait = 26;
			break;
		default:
			wait = 5;
			break;
	}
	HAL_Delay(wait);
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_READ_ADDRESS, 0xF6, 1, up_data, 3, HAL_MAX_DELAY);
	return ((up_data[0] << 16) + (up_data[1] << 8) + up_data[2]) >> (8 - bmp180->oss);
}

void BMP180_get_pressure(bmp180_t *bmp180)
{
	int32_t X1, X2, X3, up = BMP180_read_up(bmp180), p;
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

void BMP180_get_altitude(bmp180_t *bmp180)
{
	bmp180->altitude = 44330 * (1 - pow((bmp180->pressure / bmp180->sea_pressure), 1 / 5.255));
}

void BMP180_set_sea_pressure(bmp180_t *bmp180, uint32_t sea_pressure)
{
	bmp180->sea_pressure = sea_pressure;
}
