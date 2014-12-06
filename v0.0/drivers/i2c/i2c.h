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



char I2C_Start (I2C_TypeDef * I2C);
char I2C_Repeated_Start (I2C_TypeDef * I2C);
char I2C_Stop (I2C_TypeDef * I2C);
char I2C_Write (I2C_TypeDef * I2C, u8 Byte);
char I2C_Write_Address(I2C_TypeDef * I2C, u8 Address);
char I2C_Read (I2C_TypeDef * I2C, ACK_Type Ack, u8 *Byte);

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
*	\param u8 trame_received => the value of the peipheral register returned at this addres
* \return 1 if done correctly, 0 if not
*/
char single_read_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 * trame_received);

/** 
* \brief Function sending a 8 bit command to an I2C peripheral
* \param I2C_TypeDef (I2C1, I2C2, ...) => the I2C used by the peripheral
* \param u8 slave_addr => peripheral address on 7 bits
* \param u8 command => the command to send
* \return 1 if done correctly, 0 if not
*/
char send_command_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 command);

/** 
* \brief Function reading several byte on a I2C bus
* \param I2C_TypeDef (I2C1, I2C2, ...) => the I2C used by the peripheral
* \param u8 slave_addr => peripheral address on 7 bits
* \param u8 register_addr => the address of the peripheral register
* \param u8 nb_trame => the number of byte to receive from slave (must be >1, if not use the single read function)
*	\param u8 trame_received => the value of the peipheral register returned at this address (address of the table)
* \return 1 if done correctly, 0 if not
*/
char several_read_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 nb_trame, u8 ** table_received);

#endif
