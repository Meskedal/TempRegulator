/*
 * ntc.c
 *
 * Created: 18.12.2017 16:07:43
 *  Author: Eskedal
 */ 

 #include "utilities.h"
 #include "adc.h"

 
 void ntc_init(void){
	
	cli(); // Disable global interrupts
	TIMSK=(1<<TOIE1); // enable timer overflow interrupt for Timer 1
	TCCR1B = (1<<CS10)|(1<<CS12); // start timer1 with /256 prescaler
	sei(); // Enable global interrupts

 }