/*
 * usart_driver.c
 *
 * Created: 14.12.2017 16:50:13
 *  Author: Eskedal
 */ 

 #include "usart_driver.h"
 #include "utilities.h"
 #define BAUD 9600

 void usart_init(void){
	uint16_t ubrr = F_CPU / 16 / BAUD - 1;
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;

	UCSRB = (1<<RXEN)|(1<<TXEN); // Enable receiver and transmitter
	UCSRC = (1<<URSEL)|(3<<UCSZ0); // Set frame format: 8data, 1stop bit
	//clear_bit(UCSRC, 0); // Clock polarity: set to zero when using asynchronous

 }

 void USART_Transmit( unsigned char data )
 {
	 while ( !( UCSRA & (1<<UDRE)) );// Wait for empty transmit buffer
	 UDR = data; // Put data into buffer, sends the data
 }

 unsigned char USART_Receive( void )
 {
	 while ( !(UCSRA & (1<<RXC)) );// Wait for data to be received
	 return UDR; // Get and return received data from buffer
 }

 void USART_test(void){
	 char hei[10];
	 scanf("%s",hei);
	 printf("%s\n",hei);
 }