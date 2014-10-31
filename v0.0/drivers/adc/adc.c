#include "adc.h"

float Init_ADC_Single_Conv (ADC_TypeDef * ADC, u8 channel, u8 n_cycles){
	float result = 0.0;
	//turn ON clock ADCx
	if (ADC==ADC1){
		(RCC->APB2ENR) |= RCC_APB2ENR_ADC1EN;
	}else if (ADC==ADC2){
		(RCC->APB2ENR) |= RCC_APB2ENR_ADC2EN;
	};
		
	//Select channel regular sequence (single channel)
	ADC->SQR3=0x00;
	ADC->SQR3|=channel;
	
	//select number of cycles of conversion
	if (channel<10){
		ADC->SMPR2 &= ~ (0b111<<(channel*3));
		ADC->SMPR2 |=  n_cycles <<(channel*3);
	}else if (channel<17){
		ADC->SMPR1 &= ~ (0b111<<((channel-10)*3));
		ADC->SMPR1 |=  n_cycles <<((channel-10)*3);
	}
	
	//enable ADCx
	ADC->CR2|=0x01;
	
	ADC1->CR2|=0x01;	//2 times
	
	//waiting for EOC flag
	while ((ADC1->SR & 0x02) == 0x00){
		}
	//take result and clear flag
	result=ADC1->DR;
		
	return result;	
}

//fonction valeur absolue d'un float
float abs_float(float a){
	if (a<0.0){
		a=-a;
	}
	return a;
}
