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
float ntc_get_temp_B(void);
void temp_print(float temp);
float steinhart(float R);
float ntc_get_temp_SH(void);
float steinhart_SH(float R);



#endif /* NTC_H_ */