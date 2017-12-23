/*
 * usart_driver.c
 *
 * Created: 14.12.2017 16:50:13
 *  Author: Eskedal
 */ 

 #include "usart_driver.h"
 #define BAUD 115200
// #define ubrr F_CPU/(16*(BAUD-1))
/* #define ubrr 24*/

 void usart_init(void){
	/* Set baud rate */
	uint8_t ubrr = 16; //F_CPU = 16MHz, BAUD 115200, U2X = 1
	UBRRH &= ~(1 << URSEL);
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	
	/*Enable double speed*/
	UCSRA |= (1 << U2X); 
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

 void USART_flush(uint8_t N){
	char buffer[40];
	for (uint8_t i = 0; i < N; i++){
		scanf("%s", buffer);
		//printf("Flushed: %s\n",buffer);
	}
 }