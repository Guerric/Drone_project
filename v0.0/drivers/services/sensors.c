#include "sensors.h"


/*Init the right i2c bus at 400kHz and select the function to call on each byte reveived*/

void init_sensors_bus(void){
	init_i2c_400kHz (SENSORS_I2C);
}


/*Functions for updating coordinates of the drone*/

void read_x_translation(u8 * translation){
	read_i2c (SENSORS_I2C, MPU6050_ADDR, 0x3C, translation);
}
