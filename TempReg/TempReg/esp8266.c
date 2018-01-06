/*
 * esp8266.c
 *
 * Created: 19.12.2017 16:11:16
 *  Author: Eskedal
 */ 

 #include "utilities.h"
 #include "esp8266.h"
 #include "usart_driver.h"

void esp8266_init(void){
	//ESP8266 interrupts on INT0 when requesting data to be transmitted
	cli();
	MCUCR |= (1<<ISC01)|(1<<ISC00);
	GICR |= (1<<INT0);
	sei();
} 

