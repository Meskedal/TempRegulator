/*
 * TempReg.c
 *
 * Created: 14.12.2017 13:55:30
 * Author : Eskedal
 */ 
#include "utilities.h"
#include "usart_driver.h"
#include "adc.h"


int main(void){	
	DDRB |= (1 << PINB1);
	DDRC |= (1 << PINC1);
	usart_init();
	adc_init();
	float temp;

	printf("Hei");
	
    while (1) 
    {	
		//printf("hei\n");
//   		_delay_ms(100);
//   		PORTB |= (1 << PINB1);
//   		_delay_ms(100);
//   		PORTB &= ~(1 << PINB1);


// 		if(!(ADCSR & (1 << ADSC))){
// 			adc_start_conversion();
// 		} 
		if((ADCSRA & (1 << ADIF))){ // AC Interrupt flag set
			temp = adc_get_result();
			printf("Temp: %2d Celsius \n", (uint16_t)(temp*((float)100)));
		}
    }
}

