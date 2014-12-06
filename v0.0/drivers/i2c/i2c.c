/** 
* \file i2c.c
* \brief STM32F1xx I2C Driver
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS feat Emeric LASNIER
*
* Copyright (c) 2014, All rights reserved.
*
*/

#include "i2c.h"

char I2C_Init_Clock (I2C_TypeDef * I2C){
	char i=1;
	if (I2C==I2C1){
		(RCC->APB1ENR)|= RCC_APB1ENR_I2C1EN;
	}else if (I2C==I2C2){
		(RCC->APB1ENR)|= RCC_APB1ENR_I2C2EN;
	}else{
		i=0;
	}
	return i;
}

char I2C_I2C_IsIdle (I2C_TypeDef * I2C) {

  // Check if the bus is Busy or not
  if (I2C->SR2 & I2C_SR2_BUSY)
    return 1;
  else
    return 0;
}

void init_i2c_master (I2C_TypeDef * I2C, unsigned int frequency){
	unsigned int freq;
	u16 freq_range,result;
	/*Deux I2C possibles sur le stm32f103 : 
	I2C1 : PB6 (SCL) et PB7 (SDA)
	I2C2 : PB10 (SCL) et PB11 (SDA)*/
	
	Port_IO_Init_Clock (GPIOB); //initalisation de l'horloge sur le port B
	
	I2C_Init_Clock (I2C);	//clock I2C initialization
	
	//Initialisation des IO en Alternate Function Open Drain
	if (I2C==I2C1){
		Port_IO_Init_Output (GPIOB, 6, AF_OPENDRAIN, OUT_10MHZ);
		Port_IO_Init_Output (GPIOB, 7, AF_OPENDRAIN, OUT_10MHZ);
	/* Enable I2C1 reset state */
  RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
  /* Wait the reset to be done */
//  Wait_X_secondes(1);
  /* Release I2C1 from reset state */
  RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
	}else if (I2C==I2C2){
		Port_IO_Init_Output (GPIOB, 10, AF_OPENDRAIN, OUT_10MHZ);
		Port_IO_Init_Output (GPIOB, 11, AF_OPENDRAIN, OUT_10MHZ);
	/* Enable I2C2 reset state */
  RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;
  /* Release I2C2 from reset state */
  RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;
	}
	// Disable Peripheral For configuration
  I2C->CR1 &= ~I2C_CR1_PE;
  // I2C Interface not under Reset
  I2C->CR1 &= ~ I2C_CR1_SWRST;
  // No Clock Stretching
  I2C->CR1 |= I2C_CR1_NOSTRETCH;
  // I2C Mode
  I2C->CR1 &= ~ I2C_CR1_SMBUS;
	
	//Get PCLK1
	freq=CLOCK_GetPCLK1();
  freq_range = (u16)(freq / 1000000);
	//set frequency in MHZ (36MHz)
  I2C->CR2 = 0xA;//freq_range;	//10Mhz for 400kHz communication
	
	 // Set Link Speed -> Standard mode 100 kHz
  //I2C->CCR &= ~ I2C_CCR_FS;
	
  // Set Link Speed -> Fast mode 400kHz
  I2C->CCR |=  I2C_CCR_FS;
	I2C->CCR |=  I2C_CCR_DUTY;
	
  // Set the Baudrate
  result = (u16)(freq / (frequency << 1));
  if (result < 0x04)
    result = 0x04;  /* Set minimum allowed value */

  I2C->CCR |= 0xC; //result;

  /* Set Maximum Rise Time for standard mode */
  /* 1 000 ns for Standard mode */
  /* 1 000 / (1/36 Mhz) = 36 = freqrange */
  I2C->TRISE = 0x2;//freq_range + 1;

	//Enable peripheral 
	I2C->CR1 |= I2C_CR1_ACK; 	//aquitement on
	I2C->CR1 |= I2C_CR1_PE;
}


/* Basics function for I2C communication*/

char I2C_Start (I2C_TypeDef * I2C) {

  char retval = 0;
  uint32_t timeout = TIMEOUT_VALUE;

  // Check if the Bus is Idle first
  while ((I2C_I2C_IsIdle(I2C)==1) && (-- timeout != 0));

  if (timeout == 0) {
    retval = -1;
  } else {
    timeout = TIMEOUT_VALUE;
    // Generate a Start Condition
    I2C->CR1 |= I2C_CR1_START;
    // Wait for start to be generated
    while (!(I2C->SR1 & I2C_SR1_SB) && (-- timeout != 0));
        if (timeout == 0)
            retval = -1;
  }
  return retval;
}

char I2C_Repeated_Start (I2C_TypeDef * I2C) {

  char retval = 0;
  uint32_t timeout = TIMEOUT_VALUE;

	 // Check if the Bus is Idle first
  //while ((I2C_I2C_IsIdle(I2C)==1) && (-- timeout != 0));
	
  // Check if we are in Master Mode I2C for Repeated Start
  if (I2C->SR2 & I2C_SR2_MSL) {
    I2C->CR1 |= I2C_CR1_START;
    // Wait for start to be generated
    while ((!(I2C->SR1 & I2C_SR1_SB)) && (-- timeout != 0));
    if (timeout == 0)
      retval = -1;
  } else {
    retval = -1;
  }
  return retval;
}


char I2C_Stop (I2C_TypeDef * I2C) {

  char retval = 0;
  uint32_t timeout = TIMEOUT_VALUE;

  // Check if it is a transmitter or Receiver mode
  if (I2C->SR2 & I2C_SR2_TRA) {
    // Transmitter - Check if we can send a Stop Condition
    if (I2C->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) {
      // Generate a Stop Condition on the I2C Bus
      I2C->CR1 |= I2C_CR1_STOP;
      // Wait for the stop condition to be done
      while ((I2C->CR1 & I2C_CR1_STOP) && (-- timeout != 0));
        if (timeout == 0)
          retval = -1;
    } else {
      retval = -1;
    }
  } else {
    // Receiver
    // Generate a Stop Condition on the I2C Bus
    I2C->CR1 |= I2C_CR1_STOP;
    // Wait for the stop condition to be done
    while ((I2C->CR1 & I2C_CR1_STOP) && (-- timeout != 0));
      if (timeout == 0)
        retval = -1;
  }
  return retval;
}

char I2C_Write (I2C_TypeDef * I2C, u8 Byte){

  char retval = 0;
  uint32_t timeout = TIMEOUT_VALUE;

  // Write Byte
  I2C->DR = Byte;

  // Wait for the data register empty or Byte Transfer Finished
  while ((!(I2C->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF))) && (-- timeout != 0));
  if (timeout == 0)
    retval = -1;
  return retval;
}

char I2C_Write_Address(I2C_TypeDef * I2C, u8 Address){

  char retval = 0;
  uint32_t timeout = TIMEOUT_VALUE;
  uint32_t reg_value = 0;

  // Write the Address of the I2C Slave
  I2C->DR = Address;
  // Wait for ADDR to be sent
  while (!(I2C->SR1 & I2C_SR1_ADDR) && !(I2C->SR1 & I2C_SR1_AF) && (-- timeout != 0));
  // Check if there is an error on sending Address
  if (I2C->SR1 & I2C_SR1_AF || timeout == 0)
    retval = -1;
  // Read of the Status Register 2 (waited by CPU for clearing ADDR Flag)
  reg_value = I2C->SR2;
	reg_value |= 0x0;
	
  return retval;
}

char I2C_Read (I2C_TypeDef * I2C, ACK_Type Ack, u8 *Byte) {

  char retval = 0;
  uint32_t timeout = TIMEOUT_VALUE;
	
	// Check if the Bus is Idle first
 // while ((I2C_I2C_IsIdle(I2C)==1) && (-- timeout != 0));

  if (Ack == ACK) {
    // Send ACK After receiving the next Byte
    I2C->CR1 |= I2C_CR1_POS;
    // Set Acknowledgement
    I2C->CR1 |= I2C_CR1_ACK;
  } else if (Ack==NACK) {
    // Send NACK while receiving the next Byte
    I2C->CR1 &= ~I2C_CR1_POS;
    // Reset Acknowledgement
    I2C->CR1 &= ~I2C_CR1_ACK;
  }
  // Wait for the data register empty
  while (((I2C->SR1 & I2C_SR1_RXNE)==0) && (-- timeout != 0));
  if (timeout == 0){
    retval = -1;
  }else{
    *Byte = I2C->DR;
		while((I2C->SR1 & I2C_SR1_RXNE)>0);
	}
  return retval;
}

/* end of basic function section*/


char single_write_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 data){
	int i;
	i=I2C_Start (I2C);
	if (i==0){
	i=I2C_Write_Address(I2C,(slave_addr<<1));
	}
	if (i==0){
	i=I2C_Write (I2C, register_addr);
	}
	if (i==0){
	i=I2C_Write (I2C, data);
	}
	if (i==0){
	i=I2C_Stop (I2C);
	}
	return i;
}


char single_read_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 * trame_received){
	char i;	
	i=I2C_Start (I2C);
	if (i==0){
	i=I2C_Write_Address(I2C,(slave_addr<<1));
	}
	if (i==0){
	i=I2C_Write (I2C, register_addr);
	}
	if (i==0){
	i=I2C_Repeated_Start (I2C);
	}
	if (i==0){
	i=I2C_Write_Address(I2C,(slave_addr<<1)+1);
	}
	if (i==0){
	i=I2C_Stop (I2C);
	}
	if (i==0){
	i=I2C_Read (I2C, NACK, trame_received);
	}
	return i;
}

char send_command_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 command){
	char i;
	i=I2C_Start (I2C);
	if (i==0){
	i=I2C_Write_Address(I2C,(slave_addr<<1));
	}
	if (i==0){
	i=I2C_Write (I2C, command);
	}
	if (i==0){
	i=I2C_Stop (I2C);
	}
	return i;
}

char several_read_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 nb_trame, u8 ** table_received){
	char i;
	u8 j=0;
	u8 trame;
	u8 table[nb_trame];

	i=send_command_i2c (I2C,slave_addr, register_addr);
	
	i=I2C_Start (I2C);
	
	if (i==0){
	i=I2C_Write_Address(I2C,(slave_addr<<1)+1);
	}
	
	while (nb_trame>1){

    I2C->CR1 |= I2C_CR1_ACK;
		
		// Wait for the data register empty
		while ((I2C->SR1 & I2C_SR1_RXNE)==0);
    trame = I2C->DR;
		while((I2C->SR1 & I2C_SR1_RXNE)>0);
		
		table[j]=trame;
		nb_trame--;
		j++;
	}
	
	if (i==0){
		i=I2C_Read (I2C, NACK, & trame);
	}
		
	if (i==0){
	i=I2C_Stop (I2C);
	}
	
	table[j]=trame;

	*table_received=table;
	
	return i;	
}