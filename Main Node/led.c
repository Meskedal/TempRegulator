/*
 * led.c
 *
 * Created: 18.12.2017 13:03:05
 *  Author: Eskedal
 */ 

 #include "utilities.h"
 #include "led.h"
 static uint8_t counter = 0;

 #define BLINK_DELAY 1000
 void led_init(void){
	DDRB |= (1 << PINB0);
 }
 void led_blink(void){
	counter++;
	if (counter != 1){
		counter = 0;
		PORTB |= (1 << PINB0);

	} else{
		PORTB &= ~(1 << PINB0);
	}
	 _delay_ms(BLINK_DELAY);
 }