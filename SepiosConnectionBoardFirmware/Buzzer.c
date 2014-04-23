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

void buzzerPlayN(uint8_t n)
{
	uint8_t i;
	for (i = 0; i < n; i++)
	{
		buzzerPlayOne();
	}
}

void buzzerPlayFastOne() 
{
	PORTB |= (1<<PB4);
	_delay_ms(100);
	PORTB &= ~(1<<PB4);
	_delay_ms(100);
}

void buzzerPlayFastN( uint8_t n )
{
	uint8_t i;
	for (i = 0; i < n; i++)
	{
		buzzerPlayFastOne();
	}
}

void buzzerPlayLong()
{
	PORTB |= (1<<PB4);
	_delay_ms(1000);
	PORTB &= ~(1<<PB4);
	_delay_ms(1000);
}

// play a xxx number with the buzzer
void buzzerPlayNumber(uint16_t number)
{
	uint8_t digits[3];
	
	digits[0] = number % 10;
	number /= 10;
	digits[1] = number % 10;
	number /= 10;
	digits[2] = number % 10;
	
	if (digits[2])
		buzzerPlayN(digits[2]);
	else
		buzzerPlayLong();
		
	_delay_ms(1000);
	
	if (digits[1])
		buzzerPlayN(digits[1]);
	else
		buzzerPlayLong();
	
	_delay_ms(1000);
	
	if (digits[0])
		buzzerPlayN(digits[0]);
	else
		buzzerPlayLong();
		
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

void buzzerClear()
{
	buzzerOff();
	DDRB &= ~(1<<DDB4);
}
