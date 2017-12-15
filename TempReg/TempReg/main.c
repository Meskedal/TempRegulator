/*
 * TempReg.c
 *
 * Created: 14.12.2017 13:55:30
 * Author : Eskedal
 */ 
#include "utilities.h"
#include "usart_driver.h"


int main(void)
{
	usart_init();
	DDRB |= (PINB1 << 2);
	
    while (1) 
    {
 		_delay_ms(100);
 		PORTB |= (PINB1 << 2);
 		_delay_ms(100);
 		PORTB &= ~(PINB1 << 2);
		//printf("Hei");
		//USART_test();
    }
}

