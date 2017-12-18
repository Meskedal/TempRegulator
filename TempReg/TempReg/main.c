/*
 * TempReg.c
 *
 * Created: 14.12.2017 13:55:30
 * Author : Eskedal
 */ 
#include "utilities.h"
#include "led.h"
#include "usart_driver.h"
#include "adc.h"


int main(void){	
	led_init();
	usart_init();
	adc_init();
	float temp;

	
    while (1){	
   	   
	   led_blink();

		if(adc_start_MM_conversion()){
 			temp = adc_get_MM_result()-1.21; // Offset
 			printf("Temp: %2d Celsius \n", (uint16_t)(temp*((float)100)));
		}
    }
}

