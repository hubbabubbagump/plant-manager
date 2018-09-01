#ifndef _PLANT_H_
#define _PLANT_H_

#include <stdbool.h>
#define HUMIDITY "humidity"
#define TEMPERATURE "temperature"
#define LIGHT "light"
#define SLEEP_TIME_MOISTURE 2

bool spoofSensorData();

void analyzeMoisture(void);

void triggerPlantWater(int length);

float getTemperature(void);
float getHumidity(void);
double getLight(void);
float getMoisture(void);
void quitPlantManager();

#endif