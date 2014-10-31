#include "gpio.h"

//fonction qui permet d'activer le clock d'un port (necessaire à son utilisation)

char Port_IO_Init_Clock (GPIO_TypeDef* Port){
	char i=0;
	if (Port==GPIOA){
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
	}else if (Port==GPIOB){
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
	}else if (Port==GPIOC){
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
	}else{
		i=-1;
	}
	return i;
}

//Fonction qui permet de configurer un port en output push pull

char Port_IO_Init_Output (GPIO_TypeDef* Port, u8 Broche){
	if (Broche >= 8){
		Port->CRH &= ~(0xF << ((Broche-8)*4));		
		Port->CRH |= (0x1 << ((Broche - 8)*4)) & (0xF<< ((Broche-8)*4));		
	}else{
		Port->CRL &= ~(0xF << (Broche*4));
		Port->CRL |= (0x1 << (Broche*4)) & (0xF<< (Broche*4));
	}	
	return 1;
}

//Fonction qui permet de configurer un port en output alternate function push pull

char Port_IO_Init_AF_Output (GPIO_TypeDef* Port, u8 Broche){
	if (Broche >= 8){
		Port->CRH &= ~(0xF << ((Broche-8)*4));		
		Port->CRH |= (0x9 << ((Broche - 8)*4)) & (0xF<< ((Broche-8)*4));		
	}else{
		Port->CRL &= ~(0xF << (Broche*4));
		Port->CRL |= (0x9 << (Broche*4)) & (0xF<< (Broche*4));
	}	
	return 1;
}

//Fonction qui permet de configurer un port en output alternate function open drain

char Port_IO_Init_AFOD_Output (GPIO_TypeDef* Port, u8 Broche){
	if (Broche >= 8){
		Port->CRH &= ~(0xF << ((Broche-8)*4));		
		Port->CRH |= (0xD << ((Broche - 8)*4)) & (0xF<< ((Broche-8)*4));		
	}else{
		Port->CRL &= ~(0xF << (Broche*4));
		Port->CRL |= (0xD << (Broche*4)) & (0xF<< (Broche*4));
	}	
	return 1;
}

//Fonction qui permet de configurer une broche en input floating

char Port_IO_Init_Input (GPIO_TypeDef* Port, u8 Broche){
	if (Broche >= 8){
		Port->CRH &= ~(0xF<< ((Broche-8)*4));
		Port->CRH |= (0x4 << ((Broche - 8)*4)) & (0xF<< ((Broche-8)*4));		
	}else{
		Port->CRL &= ~(0xF<< (Broche*4));
		Port->CRL |= (0x4 << (Broche*4)) & (0xF<< (Broche*4));
	}	
	return 1;
}


//Fonction qui permet de configurer une broche en alternate function ADC
char Port_IO_Init_ADC (GPIO_TypeDef* Port, u8 Broche){
	if (Broche >= 8){
		Port->CRH &= ~(0xF<< ((Broche-8)*4));		
	}else{
		Port->CRL &= ~(0xF<< (Broche*4));
	}	
	return 1;
}


//Fonction qui permet de mettre 1 à une broche d'un port

void Port_IO_Set(GPIO_TypeDef* Port, u8 Broche){
	Port->ODR |= 0b1<<Broche;	
}

//Fonction qui permet de mettre 0 à une broche d'un port

void Port_IO_Reset(GPIO_TypeDef* Port, u8 Broche){
	Port->ODR &= ~(0b1<<Broche);	
}

//Fonction qui inverse l'état d'une broche
void Port_IO_Blink(GPIO_TypeDef* Port, u8 Broche){
	if ((Port->ODR & 0b1<<Broche)==0){
		Port_IO_Set(Port, Broche);			
	}else{
		Port_IO_Reset(Port, Broche);
	}	
}

//Fonction qui permet de lire l'état d'une broche d'un port (configuré en input)

unsigned int Port_IO_Read (GPIO_TypeDef * Port, u8 Broche){
	return ((Port->IDR & 0b1<<Broche)>0);
}