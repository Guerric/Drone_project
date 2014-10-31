#ifndef GPIO_H
#define	GPIO_H

#include "stm32f10x.h"

//fonction qui permet d'activer le clock d'un port (necessaire à son utilisation)

char Port_IO_Init_Clock (GPIO_TypeDef* Port);

//Fonction qui permet de configurer un port en output push pull

char Port_IO_Init_Output (GPIO_TypeDef* Port, u8 Broche);

//Fonction qui permet de configurer un port en output alternate function push pull

char Port_IO_Init_AF_Output (GPIO_TypeDef* Port, u8 Broche);

//Fonction qui permet de configurer un port en output alternate function open drain

char Port_IO_Init_AFOD_Output (GPIO_TypeDef* Port, u8 Broche);

//Fonction qui permet de configurer une broche en input floating

char Port_IO_Init_Input (GPIO_TypeDef* Port, u8 Broche);


//Fonction qui permet de configurer une broche en alternate function ADC

char Port_IO_Init_ADC (GPIO_TypeDef* Port, u8 Broche);
	

//Fonction qui permet de mettre 1 à une broche d'un port

void Port_IO_Set(GPIO_TypeDef* Port, u8 Broche);


//Fonction qui permet de mettre 0 à une broche d'un port

void Port_IO_Reset(GPIO_TypeDef* Port, u8 Broche);


//Fonction qui inverse l'état d'une broche

void Port_IO_Blink(GPIO_TypeDef* Port, u8 Broche);


//Fonction qui permet de lire l'état d'une broche d'un port (configuré en input)

unsigned int Port_IO_Read (GPIO_TypeDef * Port, u8 Broche);

#endif