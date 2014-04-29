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
		// If system was not ON (meaning OFF or FAILURE) last time, switch it ON.
		if (loadSystemState() != SYSTEM_ON)
		{
			saveSystemState(SYSTEM_ON);
			buzzerPlayTriple();
			systemSSROn();
		}
		// If system was ON, switch it OFF.
		else
		{
			saveSystemState(SYSTEM_OFF);
			systemSSROff();
			buzzerPlayLong();
		}
	
		int lowVoltageCount = 0; // Counts to LOWVOLTAGECOUNTERLIMIT to prevent system from shuting-down on short voltage drops (working like a low pass filter, integrator).
	
	
		// entering main loop.
		while(1)
		{
			_delay_ms(1000);
			
			// routinely check for low voltage. And 
			if (batteryIsLowVoltage())
			{
				buzzerPlayFastN(8);
				lowVoltageCount++;
			}
			else
			{
				lowVoltageCount = 0;

				if (batteryIsBelowWarningVoltage())
				{
					buzzerPlayN(4);
				}
			}
			
			// if lowVoltageCount exceeded LOWVOLTAGECOUNTERLIMIT switch system off.
			if (lowVoltageCount >= LOWVOLTAGECOUNTERLIMIT)
			{
				buzzerPlayLong();
				saveSystemState(SYSTEM_FAILURE);
				break;
			}
		}
	}

poweroff:
	buzzerClear();
	systemSSROff();
	batteryClear();
}


