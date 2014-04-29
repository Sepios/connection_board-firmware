/*
 * State.c
 *
 * Created: 15.04.2014 17:54:56
 *  Author: marwegma
 */ 



#include "StateManagement.h"

#include <avr/eeprom.h>

enum systemState loadSystemState()
{
	return (systemState)eeprom_read_byte((uint8_t*)SYSTEM_STATE_ADDR);
}

void saveSystemState(systemState state)
{
	eeprom_write_byte ((uint8_t*)SYSTEM_STATE_ADDR, state);
}