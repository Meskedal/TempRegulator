/*
 * usart_driver.c
 *
 * Created: 14.12.2017 16:50:13
 *  Author: Eskedal
 */ 

 #include "usart_driver.h"
 #include "utilities.h"
 #define BAUD 9600
 #define ubrr F_CPU/16/BAUD-1
/* #define ubrr 24*/

 void usart_init(void){
	/* Set baud rate */
	
// 	uint8_t uh = (unsigned char)((ubrr) >> 8);
// 	uint8_t ul = (unsigned char)(((ubrr) << 8) >> 8);
 	UBRRH = (unsigned char)((ubrr) >> 8);
 	UBRRL = (unsigned char)(((ubrr) << 8) >> 8);
 
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
	UCSRC &= ~(UCPOL << 1); //polarity
	fdevopen(USART_Transmit, USART_Receive);
// 	printf("UBRH: %d \t", uh);
// 	printf("UBRL: %d \n", ul);
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