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
#include "ntc.h"
#include "esp8266.h"


int main(void){	
	led_init();
	usart_init();
	adc_init();
	esp8266_init();
	ntc_init();
	float temp;

    while (1){	
	   led_blink();
		if(ntc_start_conv()){
 			temp = ntc_get_temp()-1.21; // Offset
 			//printf("Temp: %2d Celsius \n", (uint16_t)(temp*100.0));
		}
    }
}

