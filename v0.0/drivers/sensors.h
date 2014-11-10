#ifndef SENSORS_H
#define SENSORS_H

#include "i2c.h"

/*Select I2C bus for sensors and the interrupt priority for trame reception (lowest priority = highest priority)*/

#define SENSORS_I2C I2C1
#define SENSORS_IRQ_PRIORITY 5


/*Sensors addresses on the I2C bus*/

#define MPU6050_ADDR 105
#define ALTIMETER_ADDR 119
#define MAGNETOMETER_ADDR 30

/*Init the right i2c bus at 400kHz and select the function to call on each byte reveived*/

void init_sensors_bus(void (*On_receive_function) (u8 trame));

/*Functions for updating coordinates of the drone*/

void update_x_translation(void);

#endif
