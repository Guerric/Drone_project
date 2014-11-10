#include "i2c.h"


void I2C1_EV_IRQHandler(void){
		u16 status=I2C1->SR1;
		u8 trame= I2C1->DR;		//flag cleread after reading DR and SR1
		if ((status & (0x1<<6)) > 0){ 		//if RxNE activated
			//stop bit condition
			I2C1->CR1 |= 0b1<<9;
			Function_I2C(trame);
		}
}

void init_i2c_400kHz (I2C_TypeDef * I2C, void (*On_receive_function) (u8 trame),u8 Priority){
	/*Deux I2C possibles sur le stm32f103 : 
	I2C1 : PB6 (SCL) et PB7 (SCA)
	I2C2 : PB10 (SCL) et PB11 (SCA)*/
	
	Port_IO_Init_Clock (GPIOB); //initalisation de l'horloge sur le port B
	
	//Initialisation des IO en Alternate Function Open Drain
	if (I2C==I2C1){
		Port_IO_Init_AFOD_Output (GPIOB, 6);
		Port_IO_Init_AFOD_Output (GPIOB, 7);
	}else if (I2C==I2C2){
		Port_IO_Init_AFOD_Output (GPIOB, 10);
		Port_IO_Init_AFOD_Output (GPIOB, 11);
	}
	
	//Start peripheral frequency at 10MHz (Fast Mode, at least 4MHz and max 36MHz) Tpclk1=1/10M=0.1us=100ns
	I2C->CR2 &= ~0x3F;
	I2C->CR2 |= 0xA;
	
	//enable interrupts ITEVTEN and ITBUFEN ann setup interrupt function
	I2C->CR2 |= (0b11<<9);
	NVIC->ISER[0] |= 0b1<<31;
	NVIC->IP[31] = Priority<<4;
	On_receive_function=Function_I2C;
	
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

void read_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr){
	
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
}