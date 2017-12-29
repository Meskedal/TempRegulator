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
static uint16_t data;
static char data_send[4] = "9995";

int main(void){	
	cli();
	MCUCR |= (1<<ISC01)|(1<<ISC00);
	GICR |= (1<<INT0);
	sei();
	led_init();
	usart_init();
	adc_init();
	ntc_init();
	float temp;
    while (1){	
	   led_blink();
		if(ntc_start_conv()){
 			temp = ntc_get_temp()-1.21; // Offset
			data = (uint16_t)(temp*100.0);
			itoa(data,data_send,10);
			//printf("Data: %d\n",data);
		}
    }
}

ISR(INT0_vect){
	//printf("%d",data);
	USART_send_string(data_send,4);
}