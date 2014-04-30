/*
 * StateManagement.h
 *
 * Created: 15.04.2014 18:11:47
 *  Author: marwegma
 */ 


#ifndef STATEMANAGEMENT_H_
#define STATEMANAGEMENT_H_

#define SYSTEM_STATE_ADDR 0x00

typedef enum
{
	SYSTEM_ON = 0x00,
	SYSTEM_OFF = 0x01,
	SYSTEM_FAILURE = 0x02, // low voltage, leakage, high current, collision, etc. may apply to this state.
	SYSTEM_UNDEFINED = 0x03
} system_state;

system_state loadSystemState();
system_state getSystemState();
void saveSystemState(system_state);

#endif /* STATEMANAGEMENT_H_ */