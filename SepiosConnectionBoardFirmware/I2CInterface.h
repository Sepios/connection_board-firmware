/*
 * I2CInterface.h
 *
 * Created: 29.04.2014 17:40:52
 *  Author: marwegma
 */


#include <stdint.h>

#ifndef I2CINTERFACE_H_
#define I2CINTERFACE_H_

// command constants
typedef enum
{ 
	NONE = 0x00,
	WARNING_OFF = 0x40,
	WARNING_ON = 0x41,
	POWER_OFF = 0xf0,
} i2c_command;

void i2cInterfaceInit();
i2c_command i2cInterfacePoll();
void i2cInterfaceSendVoltage(uint8_t voltage);

#endif /* I2CINTERFACE_H_ */
