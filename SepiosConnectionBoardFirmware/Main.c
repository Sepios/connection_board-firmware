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

static volatile uint8_t alarm_external = 0; // Indicate whether the buzzer was set external by the i2c interface.


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
		TCCR1 = (1<<CS02 | 1<<CS00); // Set timer0a to 1/64 of system clock (8Mhz).
		TIMSK |= (1<<TOIE1); // Enable overflow interrupt.
		
		system_state state = loadSystemState();
		// If system was not ON and not FAILURE last time, switch it ON. The check on FAILURE prevents switching on after failure and program crash.
		if (state != SYSTEM_ON && state != SYSTEM_FAILURE)
		{
			saveSystemState(SYSTEM_ON);
			buzzerPlayTriple();
			sei();
			systemSSROn();
			
			// entering main loop.
			while(1)
			{
				sleep_mode();
				
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
							alarm_external = 1;
							buzzerSetAlarmState(BUZZER_ALARM_SLOW);
							break;
						case WARNING_OFF:
							alarm_external = 1;
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
			buzzerPlayLong();
			sei();
		}

//poweroff:
		systemSSROff();
			
		while(1)
		{
			sleep_mode();
				
			system_state state = getSystemState();
			if(state == SYSTEM_FAILURE)
			{
				break; // go to complete_poweroff;
			}
		}
	}

//complete_poweroff:
	buzzerClear();
	
	buzzerPlayLong();
	systemSSROff();
	
	batteryClear();
	
	cli(); // disable interrupts so ATtiny wont awake anymore when sleeping
	
	while(1)
	{
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode(); // go to sleep to save power. gn8. ZZZzzzzzzzz...
	}
}



// Slower non-time-critical interrupt software routine
ISR (TIMER1_OVF_vect)
{	
	static uint8_t interruptCounter = 0; // This counter diversifies the periodically timer interrupt (for slower non-time-critical section) , in the sense that it can do different things depending on this counter.
	static uint8_t buzzerCounter = 0;
	static uint8_t lowVoltageCounter = 0;
	
	// (1/(8Mhz / 64 / 256 / 128) ~) every 0.262144 seconds.
	if(!(interruptCounter % 128))
	{	
		batteryADCEnable();
		
		// Measure and handle battery
		if (batteryIsLowVoltage())
		{
			buzzerSetAlarmState(BUZZER_ALARM_FAST);
			
			lowVoltageCounter++;
			
			if (lowVoltageCounter >= LOWVOLTAGECOUNTERLIMIT) // If low voltage is detected, shutdown system after approximately 5 seconds, and only if it keeps detecting low voltage. Works like a low pass filter if there is only a short voltage drop.
			{
				alarm_external = 0;
				buzzerSetAlarmState(BUZZER_ALARM_ALWAYS);
				saveSystemState(SYSTEM_FAILURE);
			}
		}
		else
		{
			lowVoltageCounter = 0;
			if (batteryIsBelowWarningVoltage(batteryGetLastVoltage()))
			{
				alarm_external = 0;
				buzzerSetAlarmState(BUZZER_ALARM_SLOW);
			}
			else if (!alarm_external)
			{
				buzzerSetAlarmState(BUZZER_OFF);
			}		
		}
		
		batteryClear();
		
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