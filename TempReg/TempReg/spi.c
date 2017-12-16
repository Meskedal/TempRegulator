/*
 * spi.c
 *
 * Created: 15.12.2017 16:13:14
 *  Author: Eskedal
 */ 

 #include "spi.h"
 #include "utilities.h"

 void SPI_master_init(void){

	 DDRB |= (1<<DDB5)|(1<< DDB7)|(1<<DDB4);  // Set MOSI, SCK and SS output, all others input
	 SPCR |= (1<<MSTR)|(1<<SPR0)|(1<<SPE); // Enable Master, set clock rate fck/16 and enable SPI
 }