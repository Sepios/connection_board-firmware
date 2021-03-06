/*
 * I2CInterface.c
 *
 * Created: 29.04.2014 15:31:13
 *  Author: marwegma
 */

#include "I2CInterface.h"

#include "usiTwiSlave.h"

#define I2C_SLAVE_ADDR  0x26


void i2cInterfaceInit(){
	usiTwiSlaveInit(I2C_SLAVE_ADDR); // init I2C slave mode
}

i2c_command i2cInterfacePoll(){
	if (usiTwiDataInReceiveBuffer())
		return usiTwiReceiveByte();
	else
		return 0xff; // return error
}

void i2cInterfaceSendVoltage(uint8_t voltage){
	usiTwiTransmitByte(voltage);
}

