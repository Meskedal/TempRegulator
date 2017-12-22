/*
 * esp8266.c
 *
 * Created: 19.12.2017 16:11:16
 *  Author: Eskedal
 */ 

 #include "utilities.h"
 #include "esp8266.h"
 #include "usart_driver.h"

 uint8_t esp8266_init(void){
	DDRD |= (1 << PIND7);
	_delay_ms(10000);
	_delay_ms(10000);
	_delay_ms(10000);
	_delay_ms(10000);
	esp8266_simple_transmit("ATE0");//disable echo
	_delay_ms(10000);
	if(!esp8266_AT_OK()){
		return 0;
	}
	return 1;
 }
 //printf("Init Success\n");
 void esp8266_init_failure(void){
	 PORTD |= (1<< PIND7);
 }

 void esp8266_transmit(char* command){
	uint8_t success = 0;
	char response[2] = "AB";
	while(!success){
		printf("%s\r\n", command);
		scanf("%s", response);
		if(response[0] == 'O'){
			success = 1;
		}
	}
	
 }

 void esp8266_simple_transmit(char* command){
	printf("%s\r\n", command);
 }

 uint8_t esp8266_AT_OK(void){
	char response[2] = "AB";
	esp8266_simple_transmit("AT");
	scanf("%s", response);
	printf("Response: %s", response);
	if (response[0] == 'O'){
		return 1;	
	}else{
		PORTD |= (1<<PIND7);
		return 0;
	}
 }
