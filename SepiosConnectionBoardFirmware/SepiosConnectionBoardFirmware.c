/*
 * SepiosConnectionBoardFirmware.c
 *
 * Created: 04.04.2014 16:10:32
 *  Author: Nox (alias Markus Wegmann)
 */ 


#include <avr/io.h>
#include <util/delay.h>

#include "SystemSSR.h"
#include "StateManagement.h"
#include "Buzzer.h"
#include "BatteryMonitoring.h"


int main(void)
{	
	batteryInit();
	systemSSRInit();
	buzzerInit();
	
	if (batteryIsLowVoltage())
	{
		writeSystemState(SYSTEM_FAILURE);
		systemSSROff();
		buzzerPlayFastN(16);
	}
	else 
	{
		if (readSystemState() != SYSTEM_ON)
		{
			writeSystemState(SYSTEM_ON);
			buzzerPlayTriple();
			systemSSROn();
		}
		else
		{
			writeSystemState(SYSTEM_OFF);
			systemSSROff();
			buzzerPlayLong();
		}
	
		
		int lowVoltageCount = 0; // Counts to LOWVOLTAGECOUNTERLIMIT to prevent system from shuting-down on short voltage drops (like a low pass filter).
	
		while(1)
		{
			 
		
			if (batteryIsLowVoltage())
			{
				buzzerPlayFastN(8);
				lowVoltageCount++;
				systemSSROff();
			}
			else
			{
				lowVoltageCount = 0;

				if (batteryIsBelowWarningVoltage())
				{
					buzzerPlayN(4);
				}
			
			}
		
			if (lowVoltageCount >= LOWVOLTAGECOUNTERLIMIT)
			{
				buzzerPlayLong();
				writeSystemState(SYSTEM_FAILURE);
				break;
			}
			
			_delay_ms(1000);
		}
	}
	
	buzzerOff();
	systemSSROff();
	batteryClear();
}