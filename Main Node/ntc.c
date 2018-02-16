/*
 * ntc.c
 *
 * Created: 18.12.2017 16:07:43
 *  Author: Eskedal
 */ 
#include "utilities.h"
#include "adc.h"
#include "ntc.h"
#include <math.h>

#define N_READS 5
#define R0 10000.0
#define R_DIV 10000.0
#define B 3950.0
#define T0 298.15 // 25 + 273.15
#define RESOLUTION 1023.0 // 2^10
#define A_SH 0.00080912453
#define B_SH 0.00028362482
#define C_SH 0

static uint16_t readings[N_READS] = {0};
static uint8_t timer_flag = 0;



 
void ntc_init(void){
	DDRB |= (1 << PINB1) | (1 << PINB2); //Cooling, heating
	cli(); // Disable global interrupts
	TIMSK=(1<<TOIE1); // enable timer overflow interrupt for Timer 1
	TCCR1B = (1<<CS10)|(1<<CS12); // start timer1 with /1024 prescaler
	sei(); // Enable global interrupts
 }


uint8_t ntc_start_conv(void){
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

float ntc_get_temp_SH(void){
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
	temp = steinhart_SH(R);
	return temp;
}

float steinhart_SH(float R){
	float temp = log(R)*B_SH;
	temp+= A_SH;
	return temp;
}

float ntc_get_temp_B(void){
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
	//printf("R: %d\n", (uint16_t)R);
	temp = steinhart(R);
	return temp;
}

float steinhart(float R){ // 1/T = 1/T0 +1/B*log(R/R0)
	float temp = 1/B;
	temp *= log(R/R0);
	temp += 1/T0;
	temp = 1/temp;
	temp -= 273.15; // To celsius
	return temp;
}

void temp_print(float temp){
	uint16_t temp2 = (uint16_t)(temp*100.0);
	uint16_t temp2H = temp2/100;
	uint16_t temp2L = temp2;
	printf("Temp: %d,%d Celsius \n", temp2H,temp2L);
}

ISR(TIMER1_OVF_vect){
	timer_flag = 1;
}

void regulate_temp(float temp, float ref_temp){
	if (temp > ref_temp+0.3){
		PORTB &= ~(1 << PINB2);
		PORTB |= (1 << PINB1); //Cool
	} 
	else if (temp < ref_temp-0.3){
		PORTB &= ~(1 << PINB1);
		PORTB |= (1 << PINB2); //Heat
	} else{
		PORTB &= ~((1<<PINB1) | (1 << PINB2));
	}
}