/*
 * esp8266.h
 *
 * Created: 19.12.2017 16:11:28
 *  Author: Eskedal
 */ 


#ifndef ESP8266_H_
#define ESP8266_H_

 uint8_t esp8266_init(void);
 void esp8266_init_failure(void);
 void esp8266_transmit(char* command);
 void esp8266_simple_transmit(char* command);
 uint8_t esp8266_AT_OK(void);


#endif /* ESP8266_H_ */