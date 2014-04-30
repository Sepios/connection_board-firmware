/*
 * Buzzer.h
 *
 * Created: 15.04.2014 17:40:38
 *  Author: marwegma
 */ 

#ifndef BUZZER_H_
#define BUZZER_H_


#include <stdint.h>


typedef enum
{
	BUZZER_OFF = 0x00,
	BUZZER_ALARM_SLOW = 0x01,
	BUZZER_ALARM_FAST = 0x02,
	BUZZER_ALARM_ALWAYS = 0x03,
	BUZZER_NO_CHANGE = 0x04 // Do not change state of alarm (e.g. if it is set by another process).
} buzzer_state;

void buzzerInit();
void buzzerClear();
void buzzerPlayOne();
void buzzerPlayDouble();
void buzzerPlayTriple();
void buzzerPlayFastN(uint8_t n);
void buzzerPlayN(uint8_t n);
void buzzerPlayNumber(uint16_t digit);
void buzzerPlayLong();
void buzzerOn();
void buzzerOff();
buzzer_state buzzerGetAlarmState();
void buzzerSetAlarmState(buzzer_state state);
void buzzerOutputState(buzzer_state state, uint8_t counter);

#endif /* BUZZER_H_ */