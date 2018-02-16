/*
 * adc.c
 *
 * Created: 16.12.2017 17:24:34
 *  Author: Eskedal
 */ 
#include "utilities.h"
#include "adc.h"


void adc_init(void){
	DDRC |= (1 << PINC1);
	ADMUX &= ~((1 << REFS0) | (1 << REFS1) | (1 << MUX0)); // Use AREF, ADC0 
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0); //Presacler 124 F_CPU/124 = 129kHz adc 
	ADCSRA |= (1 << ADEN); // Enable ADC
}