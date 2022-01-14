Usage
=====

Adding to Your Project
^^^^^^^^^^^^^^^^^^^^^^

1. Copy `bmp180_hal.c <https://github.com/ceyhunsen/bmp180-stm32-driver/blob/main/bmp180_hal.c>`_ to your projects source directory.
2. Copy `bmp180_hal.h <https://github.com/ceyhunsen/bmp180-stm32-driver/blob/main/bmp180_hal.h>`_ to your projects include directory.
3. Change ``HAL_LIB`` definition in ``bmp180_hal.h`` (line 8) to your MCU's HAL library.
4. Compile ``bmp180_hal.c`` to an object file and link it to your main executable.

Simple Usage
^^^^^^^^^^^^

.. code-block:: c

	// Define bmp180_t struct instance
	bmp180_t bmp180;

	// Wait till initialization is complete
	while (BMP180_init(&hi2c1, &bmp180, 0));

	// Get all the values
	BMP180_get_all(&hi2c1, &bmp180, 0);
	float temperature = bmp180.temperature;
	float pressure = bmp180.pressure;
	float altitude = bmp180.altitude;
