/** 
* \file i2c.h
* \brief STM32F1xx I2C Driver
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef I2C_H
#define	I2C_H

#include "stm32f10x.h"
#include "gpio.h"
#include "clock.h"

#define SYSTEM_CLOCK        72000000            /*!< Clock System 72 000 000 MHz */
#define TIMEOUT_VALUE       SYSTEM_CLOCK/10     /*!< Timeout value 0.1 sec */


/**
 * \enum 	ACK_Type
 * \brief	ACK_Type Definitions for I2c Bus
 */
typedef enum {
    ACK=0,
    NACK=1
}ACK_Type;



/** 
* \brief Function initilizing a given I2C bus at the given frequency
* \param I2C_TypeDef (I2C1, I2C2, ...)
*	\param frequency => the frequency of the communication (must be <= 400 000Hz)
* \return 1 if done correctly, 0 if not
*/
void init_i2c_master (I2C_TypeDef * I2C, unsigned int frequency);


/** 
* \brief Function changing a peripheral register value
* \param I2C_TypeDef (I2C1, I2C2, ...) => the I2C used by the peripheral
* \param u8 slave_addr => peripheral address on 7 bits
* \param u8 register_addr => the address of the peripheral register
*	\param u8 data => the new register's value
* \return 1 if done correctly, 0 if not
*/
char single_write_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 data);


/** 
* \brief Function reading a peripheral register value
* \param I2C_TypeDef (I2C1, I2C2, ...) => the I2C used by the peripheral
* \param u8 slave_addr => peripheral address on 7 bits
* \param u8 register_addr => the address of the peripheral register
*	\param u8 trame_received => the value of the peipheral register
* \return 1 if done correctly, 0 if not
*/
char single_read_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 * trame_received);

#endif
