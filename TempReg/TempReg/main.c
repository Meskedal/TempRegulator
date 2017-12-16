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

	printf("Start_UP");
	
    while (1) 
    {	
		//printf("hei\n");
//   		_delay_ms(100);
//   		PORTB |= (1 << PINB1);
//   		_delay_ms(100);
//   		PORTB &= ~(1 << PINB1);

		//printf("Hei\n");
// 		
		if(adc_start_MM_conversion()){
			temp = adc_get_MM_result()-1.21; // Offset
			printf("Temp: %2d Celsius \n", (uint16_t)(temp*((float)100)));
		}
    }
}

