/** 
* \file gpio.c
* \brief STM32F1xx GPIO Driver
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#include "gpio.h"

char Port_IO_Init_Clock (GPIO_TypeDef* Port){
	char i=1;
	if (Port==GPIOA){
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
	}else if (Port==GPIOB){
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
	}else if (Port==GPIOC){
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
	}else if (Port==GPIOC){
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPDEN;
	}else if (Port==GPIOC){
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPEEN;
	}else{
		i=0;
	}
	return i;
}


char Port_IO_Init_Output (GPIO_TypeDef* Port, u8 Pin, OUT_Type Type, OUT_Speed Speed){
	if (Pin >= 8){
		Port->CRH &= ~(0xF << ((Pin-8)*4));		//clear configuration
		Port->CRH |= ((Type << 2) | Speed) << ((Pin-8)*4);	// set new configuration
	}else{
		Port->CRL &= ~(0xF << (Pin*4));
		Port->CRL |= ((Type<< 2) | Speed) << (Pin*4);
	}	
	return 1;
}


char Port_IO_Init_Input (GPIO_TypeDef* Port, u8 Pin, IN_Type Type){
	if (Pin >= 8){
		Port->CRH &= ~(0xF << ((Pin-8)*4));		//clear configuration
		Port->CRH |= Type << (((Pin-8)*4)+2);	// set new configuration
	}else{
		Port->CRL &= ~(0xF << (Pin*4));
		Port->CRL |= Type << ((Pin*4)+2);
	}	
	return 1;
}


void Port_IO_Set(GPIO_TypeDef* Port, u8 Broche){
	Port->ODR |= 0b1<<Broche;	
}


void Port_IO_Reset(GPIO_TypeDef* Port, u8 Broche){
	Port->ODR &= ~(0b1<<Broche);	
}


void Port_IO_Blink(GPIO_TypeDef* Port, u8 Broche){
	if ((Port->ODR & 0b1<<Broche)==0){
		Port_IO_Set(Port, Broche);			
	}else{
		Port_IO_Reset(Port, Broche);
	}	
}


char Port_IO_Read (GPIO_TypeDef * Port, u8 Broche){
	return ((Port->IDR & 0b1<<Broche)>0);
}