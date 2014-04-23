/*
 * BatteryMonitoring.h
 *
 * Created: 15.04.2014 17:43:35
 *  Author: marwegma
 */


#ifndef BATTERYMONITORING_H_
#define BATTERYMONITORING_H_

#define LOWVOLTAGECOUNTERLIMIT 5

void batteryInit();
uint8_t batteryRead();
// uint8_t batteryGetVoltage();
uint8_t batteryIsLowVoltage();
void batteryClear();
uint8_t batteryIsBelowWarningVoltage();

#endif /* BATTERYMONITORING_H_ */
