/*
 * usart_driver.h
 *
 * Created: 14.12.2017 16:50:31
 *  Author: Eskedal
 */ 


#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

void usart_init(void);
void USART_Transmit(unsigned char data);
unsigned char USART_Receive(void);
void USART_test(void);


#endif /* USART_DRIVER_H_ */