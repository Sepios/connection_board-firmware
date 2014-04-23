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
	systemSSRInit();
	buzzerInit();
	
	if (readSystemState() == SYSTEM_OFF)
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
	
    while(1)
    {
		//
    }
}