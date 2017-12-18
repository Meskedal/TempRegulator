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

 #define R0 10000.0
 #define R_DIV 10000.0
 #define B 3950.0
 #define T0 25+273.15
 #define RESOLUTION 1023.0

 void adc_init(void){
	//ADC
	DDRC |= (1 << PINC1);
	ADMUX &= ~((1 << REFS0) | (1 << REFS1) | (1 << MUX0)); // Use AREF, ADC0 
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0); //Presacler 124 F_CPU/124 = 129kHz adc 
	ADCSRA |= (1 << ADEN); // Enable ADC

	//Timer/int
	cli(); // Disable global interrupts
	TIMSK=(1<<TOIE1); // enable timer overflow interrupt for Timer 1
	TCCR1B = (1<<CS10)|(1<<CS12); // start timer1 with /256 prescaler
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
	printf("ADC_result: %d\n", ADC_result);

	float R = RESOLUTION/((float)ADC_result) - 1;
	R = R_DIV/R; 
	printf("R: %d\n", (uint16_t)R);
	float steinhart = 1/((float)B);
	steinhart *= log(R/R0);
	steinhart += 1/((float)T0);
	steinhart = 1/((float)steinhart);
	float temp =  steinhart - (float)273.15;
	ADCSRA |= (1 << ADIF); // clear flag

	return temp;
 }

 void temp_print(float temp){
	uint16_t temp2 = (uint16_t)(temp*((float)100));
	uint16_t temp2H = temp2/100;
	uint16_t temp2L = temp2;
	printf("Temp: %d,%d Celsius \n", temp2H,temp2L); 
 }

 uint8_t adc_start_MM_conversion(void){
	if(timer_flag){
		uint8_t ADC_low;
		uint16_t ADC_high;
		PORTC |= (1 << PINC1);
		_delay_us(200);
		for (uint8_t i = 0; i < N_READS; i++){
			ADCSRA |= (1 << ADSC);
			loop_until_bit_is_set(ADCSRA, ADIF);
			ADC_low = ADCL;
			ADC_high = (ADCH << 8);
			readings[i] = ADC_high | ADC_low;
		}
		PORTC &= ~(1 << PINC1);
		timer_flag = 0;
		return 1;
	}else{
		return 0;
	}
}

float adc_get_MM_result(void){
	float average = 0;
	float temp;
	float ADC_result;
	float R;
	for (uint8_t i = 0; i < N_READS; i++){
		average +=readings[i];
	}
	ADC_result = average/(float)N_READS;
	R = RESOLUTION/ADC_result - 1;
	R = R_DIV/R;
	printf("R: %d\n", (uint16_t)R);
	temp = steinhart(R);
	return temp;
}
 
 ISR(TIMER1_OVF_vect){
	timer_flag = 1;
}

void adc_test(void){
	//PORTC |= (1 << PINC1);
	adc_start_conversion();
	loop_until_bit_is_set(ADCSRA, ADIF);
	adc_get_result();
}

float steinhart(float R){ // 1/T = 1/T0 +1/B*log(R/R0)
	float temp = 1/((float)B); 
	temp *= log(R/R0); 
	temp += 1/((float)T0); 
	temp = 1/((float)temp);
	return temp - 273.15;
}