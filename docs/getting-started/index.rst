Getting Started
===============

Adding to Your Project
^^^^^^^^^^^^^^^^^^^^^^

1. Copy ``bmp180`` directory to your projects ``drivers`` directory.
2. Add ``bmp180.c`` file to your projects source files.
3. Add ``bmp180/inc/`` directory to your projects include path. 
4. Change ``I2C_LIB`` definition in ``bmp180/inc/bmp180.h`` (line 14) to your MCU's I2C HAL library (e.g. ``"stm32f4xx_hal.h"``).

Units
^^^^^

* Temperature: Celsius
* Pressure: Pascal
* Altitude: Meter

Simple Usage
^^^^^^^^^^^^

.. code-block:: c

	// Define bmp180_t struct instance
	bmp180_t bmp180 = {.oversampling_setting = standart};

	// Wait till initialization is complete
	while (bmp180_init(&hi2c1, &bmp180));

	// Get all the values
	bmp180_get_all(&bmp180);
	float temperature = bmp180.temperature;
	int32_t pressure = bmp180.pressure;
	float altitude = bmp180.altitude;

More On Getting Sensor Data
^^^^^^^^^^^^^^^^^^^^^^^^^^^

There is actually 2 sensor data: temperature and pressure. Altitude is calculated from pressure data. And each one of them can be measured seperately. But each one of them needs some other data from other calculations.

* Pressure data needs some calibration values from temperature measurements.
* Altitude calculation needs pressure data.

There are seperate functions for all 3 of them but measuring them seperately may result inaccurate data. So using ``bmp180_get_all()`` function is the recommended way. But if you really need to measure them seperately, here is the recommended minimums of function calls:

Temperature
"""""""""""

.. code-block:: c

	bmp180_get_temperature(&bmp180);
	float temperature = bmp180.temperature;

Pressure
""""""""

.. code-block:: c

	bmp180_get_temperature(&bmp180);
	bmp180_get_pressure(&bmp180);
	int32_t pressure = bmp180.pressure;

Altitude
""""""""

.. code-block:: c

	bmp180_get_temperature(&bmp180);
	bmp180_get_pressure(&bmp180);
	bmp180_get_altitude(&bmp180);
	float altitude = bmp180.altitude;

Oversampling Settings
^^^^^^^^^^^^^^^^^^^^^

BMP180 offers hardware oversampling for sensor data. These are ``ultra low power``, ``standard``, ``high resolution`` and ``ultra high resolution``. Check BMP180's datasheet for detailed information about oversampling.

Changing Oversampling Setting
"""""""""""""""""""""""""""""

Oversampling setting is stored in the ``bmp180_t`` struct. If you want to change oversampling setting, you should change ``oversampling_setting`` member and call ``bmp180_init()`` function.

For example if you want to change it to ``ultra high resolution``:

.. code-block:: c

	bmp180.oversampling_setting = ultra_high_resolution;
	bmp180_init(&hi2c1, &bmp180);

.. warning::
	If you don't call ``bmp180_init()`` function after changing setting, oversampling won't change.

Sea Pressure
^^^^^^^^^^^^

Default sea pressure is 101325 pascal.

Changing Sea Pressure
"""""""""""""""""""""

Sea pressure can be changed with modifying ``sea_pressure`` member of ``bmp180_t`` struct or calling ``bmp180_set_sea_sressure()``.

.. code-block:: c

	bmp180_set_sea_pressure(&bmp180, 101400);

If you want to measure altitude from any take-off point, first measure pressure at the ground and set it as sea pressure. After that, the new altitude calculation is your altitude from ground.

.. code-block:: c

	// ...
	// Getting pressure and setting it as sea pressure
	bmp180_get_all(&bmp180);
	bmp180_set_sea_pressure(&bmp180, bmp180.pressure);
	// After take-off, measure altitude
	bmp180_get_all(&bmp180);
	float higher_altitude_than_ground = bmp180.altitude;
	// ...

I2C Interface
"""""""""""""

BMP180 sensor only supports I2C interface. So, this driver uses STM32's I2C HAL libraries. If you want to change it to LL drivers, modify ``bmp180_read()``, ``bmp180_write()`` and ``bmp180_is_ready()`` functions.
