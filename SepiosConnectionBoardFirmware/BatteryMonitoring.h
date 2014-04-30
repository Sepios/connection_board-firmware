/*
 * BatteryMonitoring.h
 *
 * Created: 15.04.2014 17:43:35
 *  Author: marwegma
 */


#ifndef BATTERYMONITORING_H_
#define BATTERYMONITORING_H_

#define LOWVOLTAGECOUNTERLIMIT 24

void batteryInit();
uint8_t batteryMeasureVoltage();
uint8_t batteryGetLastVoltage();
uint8_t batteryIsLowVoltage();
void batteryClear();
uint8_t batteryIsBelowWarningVoltage(uint8_t voltage);

#endif /* BATTERYMONITORING_H_ */
