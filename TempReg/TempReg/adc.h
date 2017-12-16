/*
 * adc.h
 *
 * Created: 16.12.2017 17:24:43
 *  Author: Eskedal
 */ 


#ifndef ADC_H_
#define ADC_H_

void adc_init(void);
void adc_start_conversion(void);
float adc_get_result(void);
void adc_start_MM_conversion(void);
void temp_print(float temp);


#endif /* ADC_H_ */