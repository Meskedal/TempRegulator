/*
 * adc.c
 *
 * Created: 16.12.2017 17:24:34
 *  Author: Eskedal
 */ 
 #include "utilities.h"
 #include "adc.h"
 #include <math.h>

 #define N_READS 5
 static uint16_t readings[N_READS] = {0};
 static uint8_t timer_flag = 0;

 #define R0 10000
 #define R_DIV 10000
 #define B 3950
 #define T0 25+273.15
 #define RESOLUTION 1023

 void adc_init(void){
	DDRC |= (1 << PINC1);
	ADMUX &= ~((1 << REFS0) | (1 << REFS1) | (1 << MUX0)); // Use AREF, ADC0 
	ADCSRA |= (1 << ADEN); // Enable ADC

	cli(); // Disable global interrupts

	TIMSK=(1<<TOIE1); // enable timer overflow interrupt for Timer 1
	
	TCCR1B = (1<<CS12); // start timer1 with /256 prescaler
	
	sei(); // Enable global interrupts


 }

 void adc_start_conversion(void){
	if (!(ADCSRA & (1 << ADIF))){ // If ADC interrupt is cleared, start conversion
		PORTC |= (1 << PINC1);
		_delay_us(20);
		ADCSRA |= (1 << ADSC); 
	}
 }

 float adc_get_result(void){

	uint8_t ADC_low = ADCL;
	uint16_t ADC_high = (ADCH << 8);
	PORTC &= ~(1 << PINC1);

	uint16_t ADC_result = ADC_high | ADC_low;

	float R = RESOLUTION/((float)ADC_result) - 1;
	R = R_DIV/R; 
	printf("R: %d\n", (uint16_t)R);
	float steinhart = 1/((float)B);
	steinhart *= log(R/R0);
	steinhart += 1/((float)T0);
	steinhart = 1/((float)steinhart);
	float temp =  steinhart - (float)273.15;
	ADCSRA |= (1 << ADIF); // clear flag

									//ADC = (Vin * 2^10)/Vref    Vin = (ADC*Vref)/2^10
	return temp;
 }

 void temp_print(float temp){
	uint16_t temp2 = (uint16_t)(temp*((float)100));
	uint16_t temp2H = temp2/100;
	uint16_t temp2L = temp2;
	printf("Temp: %d,%d Celsius \n", temp2H,temp2L); 
 }
 
//  if((ADCSRA & (ADIF << 1))){ // AC Interrupt flag set
// 	 
//  
 void adc_start_MM_conversion(void){
	uint8_t ADC_low;
	uint16_t ADC_high;
	PORTC |= (1 << PINC1);
	_delay_us(20);
	for (uint8_t i = 0; i < N_READS; i++){
		ADCSRA |= (1 << ADSC);
		loop_until_bit_is_set(ADCSRA, ADIF);
		ADC_low = ADCL;
		ADC_high = (ADCH << 8);
		readings[i] = ADC_high | ADC_low;
	}
 }
 
 ISR(TIMER1_OVF_vect){
	timer_flag = 1;
	adc_start_conversion();
}