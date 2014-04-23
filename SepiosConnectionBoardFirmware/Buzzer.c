/*
 * Buzzer.c
 *
 * Created: 15.04.2014 17:12:33
 *  Author: marwegma
 */


#include <avr/io.h>
#include <util/delay.h> 


void buzzerInit()
{
	DDRB |= (1<<DDB4);
}

void buzzerPlayOne()
{
	PORTB |= (1<<PB4);
	_delay_ms(200);
	PORTB &= ~(1<<PB4);
	_delay_ms(200);
}

void buzzerPlayDouble()
{
	buzzerPlayOne();
	buzzerPlayOne();
}

void buzzerPlayTriple()
{
	buzzerPlayOne();
	buzzerPlayOne();
	buzzerPlayOne();
}

void buzzerPlayLong()
{
	PORTB |= (1<<PB4);
	_delay_ms(1000);
	PORTB &= ~(1<<PB4);
	_delay_ms(1000);
}

void buzzerOn()
{
	PORTB |= (1<<PB4);
}

void buzzerOff()
{
	PORTB &= ~(1<<PB4);
}
