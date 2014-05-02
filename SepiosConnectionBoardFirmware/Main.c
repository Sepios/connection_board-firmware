/*
 * SepiosConnectionBoardFirmware.c
 *
 * Created: 04.04.2014 16:10:32
 *  Author: Nox (alias Markus Wegmann)
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "SystemSSR.h"
#include "StateManagement.h"
#include "Buzzer.h"
#include "BatteryMonitoring.h"
#include "I2CInterface.h"



int main(void)
{	
	batteryInit();
	systemSSRInit();
	buzzerInit();
	i2cInterfaceInit();
	
	
	// Initial check for low voltage.
	if (batteryIsLowVoltage())
	{
		saveSystemState(SYSTEM_FAILURE);
		systemSSROff();
		buzzerPlayFastN(16);
	}
	else 
	{
		// Setup slower non-time-critical section for battery management to check and handle low voltage, running in parallel a time-critical I2C interface in the main loop.
		TCCR0A = (1<<CS01, 1<<CS00); // Set timer0a to 1/64 of system clock (8Mhz).
		TIMSK |= (1<<TOIE0); // Enable overflow interrupt.
		sei();
		
		
		// If system was not ON (meaning OFF or FAILURE) last time, switch it ON.
		if (loadSystemState() != SYSTEM_ON)
		{
			saveSystemState(SYSTEM_ON);
			buzzerPlayTriple();
			systemSSROn();
			
			// entering main loop.
			while(1)
			{
				uint8_t message = i2cInterfacePoll();
				if (message != 0xff) // if message is not empty
				{
					switch (message)
					{
						case NONE:
						break;
						case POWER_OFF:
						saveSystemState(SYSTEM_OFF);
						break;
						case WARNING_ON:
						buzzerSetAlarmState(BUZZER_ALARM_SLOW);
						break;
						case WARNING_OFF:
						buzzerSetAlarmState(BUZZER_OFF);
						break;
						default:
						break;
					}
					
					i2cInterfaceSendVoltage(batteryGetLastVoltage()); // Send last measured voltage if there was a command.
				}
				
				system_state state = getSystemState();
				if(state == SYSTEM_FAILURE || state == SYSTEM_OFF)
				{
					break; // go to poweroff;
				}
			}
		}
		// If system was ON, switch it OFF.
		else
		{
			saveSystemState(SYSTEM_OFF);
			systemSSROff();
			buzzerPlayLong();
		}
	}

poweroff:
	buzzerClear();
	
	buzzerPlayLong();
	systemSSROff();
	
	batteryClear();
	
	cli(); // disable interrupts so ATtiny wont awake anymore when sleeping
	
	while(1)
	{
		sleep_mode(); // go to sleep to save power. gn8. ZZZzzzzzzzz...
	}
}


#pragma region Interrupts

// Slower non-time-critical interrupt software routine
ISR (TIMER0_OVF_vect)
{
	static uint8_t interruptCounter = 0; // This counter diversifies the periodically timer interrupt (for slower non-time-critical section) , in the sense that it can do different things depending on this counter.
	static uint8_t buzzerCounter = 0;
	static uint8_t lowVoltageCounter = 0;
	
	// (1/(8Mhz / 64 / 256 / 128) ~) every 0.262144 seconds.
	if(!(interruptCounter % 128))
	{
		// Measure and handle battery
		if (batteryIsLowVoltage())
		{
			buzzerSetAlarmState(BUZZER_ALARM_FAST);
			
			lowVoltageCounter++;
			
			if (lowVoltageCounter >= LOWVOLTAGECOUNTERLIMIT) // If low voltage is detected, shutdown system after approximately 5 seconds, and only if it keeps detecting low voltage. Works like a low pass filter if there is only a short voltage drop.
			{
				buzzerSetAlarmState(BUZZER_ALARM_ALWAYS);
				saveSystemState(SYSTEM_FAILURE);
			}
		}
		else
		{
			lowVoltageCounter = 0;
			if (batteryIsBelowWarningVoltage(batteryGetLastVoltage()))
			{
				buzzerSetAlarmState(BUZZER_ALARM_SLOW);
			}
		}
		
		
		// Set buzzer according to the state
		buzzer_state state = buzzerGetAlarmState();
		if(state != BUZZER_OFF)
		{
			buzzerOutputState(state, buzzerCounter);
			buzzerCounter++;
		}
		else
		{
			buzzerOff();
			buzzerCounter = 0;
		}
	}
		
	interruptCounter++;
}

#pragma endregion Interrupts

