#include "sensors.h"

/*Init the right i2c bus at 400kHz and select the function to call on each byte reveived*/

void init_sensors_bus(void (*On_receive_function) (u8 trame)){
	init_i2c_400kHz (SENSORS_I2C, On_receive_function,SENSORS_IRQ_PRIORITY);
}


/*Functions for updating coordinates of the drone*/

void update_x_translation(void){
	write_i2c (SENSORS_I2C, MPU6050_ADDR, 0x01, 0x01);
	read_i2c (SENSORS_I2C, MPU6050_ADDR, 0x01);
}
