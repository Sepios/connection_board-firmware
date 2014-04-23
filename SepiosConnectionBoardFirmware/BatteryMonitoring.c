/*
 * BatteryMonitoring.c
 *
 * Created: 15.04.2014 17:10:02
 *  Author: Nox (alias Markus Wegmann)
 */

#include <avr/io.h>


// Battery voltage (V) per higher bit step calculation:
//		22 kOhm / (100 kOhm + 22 kOhm) = 0.18032786885 measurement V per Volt battery (due to resistor network)
//		Full ADC value at 3.3 measurement V: (int) 255 -> 3.3 V / 255 = 0.01294117647 measurement V per bit
//
//		Battery voltage 0.01 V per bit: 0.01294117647 / 0.18032786885 * 100 = 7.176470588
#define VOLTAGE_FACTOR 7.176470588

// Calculation of lower voltage limit of LiPo battery (2 x 3.2 V = 7.4 V): 2 x 3.3 V (3.1 V min.) lower limit cell = 6.6 V. In Bits: 6.6 / 0.07176470588 = 91.9672131178 ~ 92
#define LOWER_LIMIT 92
#define WARNING_LIMIT 95

void batteryInit()
{
	// Configure ADC
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1); // Set ADC prescaler to 64 --> 125 kHz ADC clock
	ADMUX |= (1 << ADLAR) | (1<<MUX1) | (1<<MUX0); // Left align ADC register, making ADCH the higher 8 bits, and ADCL the lower 2 bits. Select ADC channel 3.
	ADCSRA |= (1<<ADEN); // Enable ADC
}

void batteryClear()
{
	ADCSRA &= ~(1<<ADEN); // Disable ADC to save power
}

uint8_t batteryRead()
{
	ADCSRA |= (1<<ADSC);
	
	// Wait for end of conversion
	while (ADCSRA & (1<<ADSC))
	{}
	
	uint8_t result = ADCH; // Read higher byte (ADCL lower byte)
	return result;
}

/* Does not work concerning floating calculus and ATtiny85
// Get battery Voltage in 0.01 V steps.
uint8_t batteryGetVoltage()
{
	return batteryRead() * VOLTAGE_FACTOR;
}
*/

uint8_t batteryIsLowVoltage()
{
	if (batteryRead() <= LOWER_LIMIT)
		return 1;
	else
		return 0;
}

uint8_t batteryIsBelowWarningVoltage()
{
	if (batteryRead() <= WARNING_LIMIT)
	return 1;
	else
	return 0;
}