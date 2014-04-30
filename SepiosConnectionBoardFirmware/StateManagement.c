/*
 * State.c
 *
 * Created: 15.04.2014 17:54:56
 *  Author: marwegma
 */ 



#include "StateManagement.h"

#include <avr/eeprom.h>

static volatile currentSystemState = SYSTEM_UNDEFINED;

system_state loadSystemState()
{
	return (system_state)eeprom_read_byte((uint8_t*)SYSTEM_STATE_ADDR);
}

system_state getSystemState()
{
	return currentSystemState;
}

void saveSystemState(system_state state)
{
	currentSystemState = state;
	eeprom_write_byte ((uint8_t*)SYSTEM_STATE_ADDR, state);
}