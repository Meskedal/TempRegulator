/*
 * esp8266.c
 *
 * Created: 19.12.2017 16:11:16
 *  Author: Eskedal
 */ 

 #include "utilities.h"
 #include "esp8266.h"
 #include "usart_driver.h"

 static uint8_t send_request = 0;

 typedef enum tag_esp8266_mode{
	STA = 1+48,
	AP = 2+48,
	APSTA = 3+48
 } esp8266_mode;
 
 #define SSID "S23"
 #define PW "hoppeslott"
 #define IP "192.168.1.91"
 void esp8266_init(void){
	_delay_ms(10000);
	_delay_ms(10000);
	esp8266_simple_transmit("AT+CIPCLOSE=0");
	_delay_ms(10000);
	esp8266_simple_transmit("ATE0");
	_delay_ms(1000);
	esp8266_simple_transmit("AT+CIPMUX=1");
	_delay_ms(1000);
	esp8266_simple_transmit("AT+CIPSERVER=1,80");
	_delay_ms(1000);
	cli();
	MCUCR |= (1<<ISC01)|(1<<ISC00);
	GICR |= (1<<INT0);
	sei();
 }

 uint8_t esp8266_setup_init(void){
	DDRD |= (1 << PIND7);
	_delay_ms(10000);
	_delay_ms(10000);
	_delay_ms(10000);
	esp8266_simple_transmit("ATE0");//disable echo
	_delay_ms(100);
	esp8266_AT_OK(); //first always fails?
	_delay_ms(100);
	if(!esp8266_AT_OK()){
		PORTD |= (1<<PIND7);
		return 0;
	}
    if(!esp8266_get_ip()){
   		esp8266_set_mode(STA);
   		if(esp8266_connect()){
   			esp8266_set_ip();
			_delay_ms(10000);
			printf("WIFI set\n");
   		}
    }
	printf("WIFI set\n");
	return 1;
 }

 void esp8266_init_failure(void){
	 PORTD |= (1<< PIND7);
 }

 void esp8266_transmit(char* command){
	uint8_t success = 0;
	char response[4] = "AB  ";
	while(!success){
		printf("%s\r\n", command);
		scanf("%s", response);
		//printf("Response: %s\n", response);
		if(response[0] == 'O'){
			return;
		}
		_delay_ms(10000);
	}
	
 }

 void esp8266_simple_transmit(char* command){
	printf("%s\r\n", command);
 }

 uint8_t esp8266_AT_OK(void){
	char response[2];
	esp8266_simple_transmit("AT");
	scanf("%s", response);
	//printf("Response: %s", response);
	if (response[0] == 'O'){
		return 1;	
	}else{
		return 0;
	}
 }


 uint8_t esp8266_get_ip(void){
	char cipsta[33];
	esp8266_simple_transmit("AT+CIPSTA?");
	scanf("%s", cipsta);
	//printf("%s\n", cipsta);
	USART_flush(3);
	if (cipsta[12] == '1'){
		esp8266_set_ip(); //Static IP adress of esp8266
		printf("Ip sucess\n");
		return 1;
	}
	return 0;
 }
 
 void esp8266_set_mode(uint8_t mode){
	char cwmode[11] = "AT+CWMODE=n";
	cwmode[10] = (char)mode;
	esp8266_transmit(cwmode);
 }

 uint8_t esp8266_connect(void){
	char msg[14 + strlen(SSID) + strlen(PW)];
	sprintf(msg, "AT+CWJAP=\"%s\",\"%s\"", SSID, PW);
	esp8266_simple_transmit(msg);
	USART_flush(1); //Flushes away WIFI
	scanf("%s", msg);
	//printf("%s\n",msg);
	if(msg[0] == 'D'){ //WIFI DISCONNECTED
		USART_flush(3);//Flush away WIFI CONNECTED WIFI
		scanf("%s", msg);
		if(msg[0] == 'G'){ // WIFI GOT IP
			USART_flush(1);
			return 1;
		}
	} else if(msg[0] == 'C'){ //WIFI CONNECTED
		USART_flush(3);
		scanf("%s", msg);
		if(msg[5] == 'G'){ // WIFI GOT IP
			USART_flush(1);
			return 1;
		}
	}
	return 0;
 }

 void esp8266_set_ip(void){
	char msg[12 + strlen(IP)];
	sprintf(msg, "AT+CIPSTA=\"%s\"", IP);
	esp8266_simple_transmit(msg);
	USART_flush(1);
 }



 void esp8266_send(uint16_t data){
	if(send_request){
		cli();
		char temp[4];
		itoa(data,temp,10);
		char send[7];
		for (uint8_t i = 0; i < 5; i++){
			if(i > 2){
				send[i] = temp[i-1];
			} else{
			send[i] = temp[i];
			}
		}
		send[2] = ',';
		send[5] = '\'';
		send[6] = 'C';
		esp8266_simple_transmit("AT+CIPSEND=0,7");
		_delay_ms(200);
		printf(send);
		_delay_ms(2000);
		esp8266_simple_transmit("AT+CIPCLOSE=0");
		_delay_ms(10000);
		_delay_ms(10000);
		esp8266_simple_transmit("AT+CIPCLOSE=0");
		_delay_ms(1000);
		GIFR |= (1<< INTF0);
		GICR |= (1<<INT0);
		sei();
	}
	
	send_request = 0;
 }

 ISR(INT0_vect){
	cli();
	GICR &= ~(1<<INT0);
	send_request = 1;
	sei();
 }