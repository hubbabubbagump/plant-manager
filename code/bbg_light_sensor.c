#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bbg_light_sensor.h"
#include "fileManager.h"

#define ADC "BB-ADC"
#define LIGHT_FILE "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define VOLTAGE_MAX 1.8
#define LIGHT_MAX_READING 4095
#define NORMALIZED_MAX_VALUE 100

//We should setup the photoresistor with A 1k ohm resistor
#define RESISTOR_STRENGTH 1000

double getLightReading()
{
	FILE *lightFile = fopen(LIGHT_FILE, "r");
	if(!lightFile){
		printf("Please turn on the ADC virtual cape with the following command:\n");
		printf("echo BB-ADC > /sys/devices/platform/bone_capemgr/slots\n");
		exit(EXIT_FAILURE);
	}

	double lightReading = 0;
	fscanf(lightFile, "%lf", &lightReading);
	fclose(lightFile);

	// if (lightReading > 0) {
	// 	double voltage = (lightReading / LIGHT_MAX_READING);
	// 	double resistance = (RESISTOR_STRENGTH / voltage) - RESISTOR_STRENGTH;
	// 	printf("Current Reading: %f, Current Voltage: %lf, Resistance Adjusted: %f\n", lightReading, voltage, resistance);
	// }
	if (lightReading < 0) {
		return -1;
	}
	

	return lightReading * NORMALIZED_MAX_VALUE / LIGHT_MAX_READING;
}

void initLightSensor() {
	writeToFile(SLOTS, ADC);
}
