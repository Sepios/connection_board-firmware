/*
 * StateManagement.h
 *
 * Created: 15.04.2014 18:11:47
 *  Author: marwegma
 */ 


#ifndef STATEMANAGEMENT_H_
#define STATEMANAGEMENT_H_

#define SYSTEM_STATE_ADDR 0x00

typedef enum systemState
{
	SYSTEM_ON,
	SYSTEM_OFF,
	SYSTEM_CRASH
} systemState;

systemState readSystemState();
void writeSystemState(systemState);

#endif /* STATEMANAGEMENT_H_ */