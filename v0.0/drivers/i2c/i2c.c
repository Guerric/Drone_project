/** 
* \file i2c.c
* \brief STM32F1xx I2C Driver
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
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

void init_i2c_400kHz (I2C_TypeDef * I2C){
	/*Deux I2C possibles sur le stm32f103 : 
	I2C1 : PB6 (SCL) et PB7 (SCA)
	I2C2 : PB10 (SCL) et PB11 (SCA)*/
	
	Port_IO_Init_Clock (GPIOB); //initalisation de l'horloge sur le port B
	
	I2C_Init_Clock (I2C);	//clock I2C initialization
	
	//Initialisation des IO en Alternate Function Open Drain
	if (I2C==I2C1){
		Port_IO_Init_Output (GPIOB, 6, AF_OPENDRAIN, OUT_10MHZ);
		Port_IO_Init_Output (GPIOB, 7, AF_OPENDRAIN, OUT_10MHZ);
	}else if (I2C==I2C2){
		Port_IO_Init_Output (GPIOB, 10, AF_OPENDRAIN, OUT_10MHZ);
		Port_IO_Init_Output (GPIOB, 11, AF_OPENDRAIN, OUT_10MHZ);
	}
	
	
	//Start peripheral frequency at 10MHz (Fast Mode, at least 4MHz and max 36MHz) Tpclk1=1/10M=0.1us=100ns
	I2C->CR2 &= ~0x3F;
	I2C->CR2 |= 0xA;
	
	//enable interrupts ITEVTEN and ITBUFEN ann setup interrupt function
	/*I2C->CR2 |= (0b11<<9);
	NVIC->ISER[0] |= 0b1<<31;
	NVIC->IP[31] = Priority<<4;
	On_receive_function=Function_I2C;
	*/
	//Select frequency communication at 400kHz
	I2C->CCR |= 0b1<<15; 	//fast mode I2C selected thigh=tlow
	I2C->CCR |= 0x07;    	//400kHz => spec MPU6050 : Thigh_min=0.6us and Tlow_min=1.3us ,   Tlow=2*CCR*Tpclk1 and Thigh=CCR*Tpclk1  , we choose Thigh = 0.7us
	
	//maximum trise time
	I2C->TRISE |= 0x11;				//1000ns/Tpclk1 =10, 10+1=11
	
	//Enable peripheral 
	I2C->CR1 |= 0b1<<10; 	//aquitement on
	I2C->CR1 |= 0x01;
	
}

void write_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 data){
	
	u16 status;
	
	//start condition
	I2C->CR1 |= 0b1<<8;
	
	//reading status register SR1 until start condition generated
	while ((I2C->SR1 & 0x01)==0){}
	
	// write slave address in data register
	I2C->DR = slave_addr<<1;
	
	//read SR1 and SR2 to clear ADDR
	while ((I2C->SR1 & (0x01<<1))==0){}
	status = I2C->SR2;
	
	//send data
	I2C->DR = register_addr;
	
	//wait until byte transfert finished
	while ((I2C->SR1 & (0x01<<2))==0){}
		
	//send data
	I2C->DR = data;
	
	//wait until byte transfert finished
	while ((I2C->SR1 & (0x01<<2))==0){}
	
	//stop bit condition
	I2C->CR1 |= 0b1<<9;		
}

void read_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 * trame_received){
	
	u16 status;
	
	//start condition
	I2C->CR1 |= 0b1<<8;
	
	//reading status register SR1 until start condition generated
	while ((I2C->SR1 & 0x01)==0){}
	
	// write slave address in data register in receive mode
	I2C->DR = (slave_addr<<1)+1;
	
	//read SR1 and SR2 to clear ADDR
	while ((I2C->SR1 & (0x01<<1))==0){}
	status = I2C->SR2;
	
	//send a NACK to stop connection after 1 byte
	I2C->CR1 &= ~0b1<<10;	
	
	//send the address of the register to read
	I2C->DR = register_addr;
	
	//wait until byte transfert finished
	while ((I2C->SR1 & (0x01<<2))==0){}		
		
	//wait for RxNE activated
	while ((I2C1->SR1 & (0x1<<6)) == 0){}
		
	*trame_received= I2C1->DR;
		
	//stop bit condition
	I2C->CR1 |= 0b1<<9;	
}