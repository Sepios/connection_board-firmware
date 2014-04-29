/*
 * I2CInterface.h
 *
 * Created: 29.04.2014 17:40:52
 *  Author: marwegma
 */ 


#ifndef I2CINTERFACE_H_
#define I2CINTERFACE_H_

// command constants
const uint8_t NONE = 0x00;
const uint8_t POWER_OFF = 0xf0;

void i2cInterfaceInit();
uint8_t i2cInterfacePoll();
void i2cInterfaceSendVoltage(uint_8 voltage);

#endif /* I2CINTERFACE_H_ */
