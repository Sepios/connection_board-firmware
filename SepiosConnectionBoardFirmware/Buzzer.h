/*
 * Buzzer.h
 *
 * Created: 15.04.2014 17:40:38
 *  Author: marwegma
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

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

#endif /* BUZZER_H_ */