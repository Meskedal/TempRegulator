/*
 * esp8266.c
 *
 * Created: 19.12.2017 16:11:16
 *  Author: Eskedal
 */ 

 #include "utilities.h"
 #include "esp8266.h"
 #include "usart_driver.h"

void esp8266_init(void){
	//ESP8266 interrupts on INT0 when requesting data to be transmitted
	cli();
	MCUCR |= (1<<ISC01)|(1<<ISC00);
	GICR |= (1<<INT0);
	sei();
} 

void esp8266_receive(void){ // Called in interupt vector
	uint8_t msg_length = atoi(USART_Receive());
	char instruction[msg_length];
	USART_receive_string(instruction, msg_length);
	if (instruction == "send"){

	}
	//Use 1 transmission that implies instruction, s = send, u = update to avoid allocatin memory

}
