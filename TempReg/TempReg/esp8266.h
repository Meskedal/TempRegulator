/*
 * esp8266.h
 *
 * Created: 19.12.2017 16:11:28
 *  Author: Eskedal
 */ 


#ifndef ESP8266_H_
#define ESP8266_H_

 void esp8266_init(void);
 uint8_t esp8266_setup_init(void);
 void esp8266_init_failure(void);
 void esp8266_transmit(char* command);
 void esp8266_simple_transmit(char* command);
 uint8_t esp8266_AT_OK(void);
 uint8_t esp8266_get_ip(void);
 void esp8266_set_mode(uint8_t mode);
 uint8_t esp8266_connect(void);
 void esp8266_set_ip(void);
 void esp8266_send(uint16_t data);


#endif /* ESP8266_H_ */