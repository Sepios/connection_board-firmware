/*
 * SystemSSR.c
 *
 * Created: 15.04.2014 17:18:06
 *  Author: marwegma
 */ 


#include <avr/io.h>


void systemSSRInit()
{
	DDRB |= (1<<DDB1);	
}

void systemSSROn()
{
	PORTB |= (1<<PB1);
}

void systemSSROff()
{
	PORTB &= ~(1<<PB1);
}