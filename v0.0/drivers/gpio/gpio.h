/** 
* \file gpio.h
* \brief STM32F1xx GPIO Driver
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef GPIO_H
#define	GPIO_H

#include "stm32f10x.h"

/**
* \enum IN_Type
*	\brief Select the input type (analog, floating...)
*/
typedef enum{
	ANALOG=0,
	FLOATING=1,
	PULL_UP_DOWN=2,
}IN_Type;


/**
* \enum OUT_Type
*	\brief Select the output type (pushpull, open drain or alternate function)
*/
typedef enum{
	PUSHPULL=0,
	OPENDRAIN=1,
	AF_PUSHPULL=2,
	AF_OPENDRAIN=3
}OUT_Type;


/**
* \enum OUT_Speed
*	\brief Select the maximum output speed
*/
typedef enum{
	OUT_10MHZ=1,
	OUT_2MHZ=2,
	OUT_50MHZ=3
}OUT_Speed;


/** 
* \brief Function activating the clock of the righ port you wqnt to configure
* \param GPIO_TypeDef (GPIOA, GPIOB ...)
* \return 1 if done correctly, 0 if not
*/
char Port_IO_Init_Clock (GPIO_TypeDef* Port);


/** 
* \brief //Configure an IO as output
* \param GPIO_TypeDef => GPIOA, GPIOB ...
* \param u8 Pin => correcponding pin
* \param OUT_Type => select the output type (pushpull, open drain ...)
* \param OUT_Speed => select the output speed (2, 10 or 50 MHz)
* \return 0 if done correctly
*/
char Port_IO_Init_Output (GPIO_TypeDef* Port, u8 Pin, OUT_Type Type, OUT_Speed Speed);


/** 
* \brief //Configure an IO as input
* \param GPIO_TypeDef => GPIOA, GPIOB ...
* \param u8 Pin => correcponding pin
* \param IN_Type => select the input type (analog, floating ...)
* \return 1 if done correctly
*/
char Port_IO_Init_Input (GPIO_TypeDef* Port, u8 Pin, IN_Type Type);
	

/** 
* \brief //Set output high
* \param GPIO_TypeDef => GPIOA, GPIOB ...
* \param u8 Pin => correcponding pin
* \return NULL
*/
void Port_IO_Set(GPIO_TypeDef* Port, u8 Broche);


/** 
* \brief //Set output low
* \param GPIO_TypeDef => GPIOA, GPIOB ...
* \param u8 Pin => correcponding pin
* \return NULL
*/
void Port_IO_Reset(GPIO_TypeDef* Port, u8 Broche);


/** 
* \brief //Change the state of an IO
* \param GPIO_TypeDef => GPIOA, GPIOB ...
* \param u8 Pin => correcponding pin
* \return NULL
*/
void Port_IO_Blink(GPIO_TypeDef* Port, u8 Broche);


/** 
* \brief //Read an IO state
* \param GPIO_TypeDef => GPIOA, GPIOB ...
* \param u8 Pin => correcponding pin
* \return 0 or 1 according the state pin
*/
char Port_IO_Read (GPIO_TypeDef * Port, u8 Broche);

#endif