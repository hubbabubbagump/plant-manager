#ifndef _CONFIG_H_
#define _CONFIG_H_

#define TEMPERATURE_ACCURACY 0.5f //hard value
#define LIGHT_ACCURACY 5 //hard value
#define HUMIDITY_ACCURACY 0.05f //percent

void initConfig();
void checkDirty();

void setWaterThresholdUpper(int value);
int getWaterThresholdUpper();

void setWaterThresholdLower(int value);
int getWaterThresholdLower();


void setTempThresholdUpper(int value);
int getTempThresholdUpper();

void setTempThresholdLower(int value);
int getTempThresholdLower();

void setHumidityThresholdUpper(int value);
int getHumidityThresholdUpper();

void setHumidityThresholdLower(int value);
int getHumidityThresholdLower();

void setExpectedLight(int value);
int getExpectedLight();

#endif