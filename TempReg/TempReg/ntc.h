/*
 * ntc.h
 *
 * Created: 18.12.2017 16:07:50
 *  Author: Eskedal
 */ 


#ifndef NTC_H_
#define NTC_H_

void ntc_init(void);
uint8_t ntc_start_conv(void);
float ntc_get_temp(void);
void temp_print(float temp);
float steinhart(float R);



#endif /* NTC_H_ */