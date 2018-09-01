#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "grove.h"
#include "bbg_dht_read.h"
#include "configuration.h"
#include "bbg_light_sensor.h"
#include "dataStore.h"
#include "plant-manager.h"
#include "networkManager.h"
#include "threadManager.h"

////////////////////////////////
// FOR DATA SPOOFING PURPOSES
////////////////////////////////

static const bool spoofData = false;

bool spoofSensorData() {
    return spoofData;
}

////////////////////////////////
// END DATA SPOOFING STUFF
////////////////////////////////

#define SLEEP_TIME_READ 60
#define DATE_LEN 26
#define DEGREE_CHAR 248
#define ERROR_LENGTH 256
#define NUM_PERIODS 3

static float numPeriods = 1;

static float temperature = -1.0f; //celcius
static float humidity = -1.0f; //percentage
static double light = -1; //percentage
static float moisture = -1.0f; //percentage

static bool quitApplication;
static bool waterPlant;
static bool isWatering;

static int waterLength = SLEEP_TIME_MOISTURE;

static void initComponents() {
    quitApplication = false;
    waterPlant = false;
    isWatering = false;
    srand(time(NULL));

    initConfig();
    initializeGrove();
    initLightSensor();
    initializeDht();
    initThreads();
}

static void closeComponents() {
    closeGrove();
    cleanThreads();
    exitThreads();
}

static void printClosingMessage() {
    fflush(stdout);
    printf("\33[2K\r");
    printf("Closing plant manager");
    for (int i = 0; i < numPeriods; i++) {
        printf(".");
    }
    if (numPeriods >= NUM_PERIODS) {
        numPeriods = 0;
    }
    else {
        numPeriods++;
    }
}

static void displayError(char *dataType) {
    char msg[ERROR_LENGTH];
    if (strcmp(dataType, TEMPERATURE) == 0) {
        int low = getTempThresholdLower();
        int upper = getTempThresholdUpper();
        sprintf(msg, "Error: the %s value of %.2f is outside the expected range of [%d - %d] +- 0.5 degrees", TEMPERATURE, temperature, low, upper);
        printf("%s\n", msg);
    }
    else if (strcmp(dataType, HUMIDITY) == 0) {
        int low = getHumidityThresholdLower();
        int upper = getHumidityThresholdUpper();
        sprintf(msg, "Error: the %s value of %.2f is outside the expected range of [%d - %d] +- 0.05%%", HUMIDITY, humidity, low, upper);
        printf("%s\n", msg);
    }
    else if (strcmp(dataType, LIGHT) == 0) {
        int expected = getExpectedLight();
        sprintf(msg, "Error: the amount of %s within the last 24 hours does not reach %d", LIGHT, expected);
        printf("%s\n", msg);
    }
}

static void analyzeBreadboardComponents() {
    if (humidity + HUMIDITY_ACCURACY < getHumidityThresholdLower() || humidity - HUMIDITY_ACCURACY > getHumidityThresholdUpper()) {
        displayError(HUMIDITY);
    }

    if (temperature + TEMPERATURE_ACCURACY < getTempThresholdLower() || temperature - TEMPERATURE_ACCURACY > getTempThresholdUpper()) {
        displayError(TEMPERATURE);
    }

    if (analyzeLightData() == true) {
        displayError(LIGHT);
    }
}

static void collectData() {
    while (quitApplication == false) { //replace with a while loop

        int result = -1;
        if (spoofData == false) {
            //Read temperature and humidity
            result = readHumidityTemp(&humidity, &temperature);

            //Read light value
            light = getLightReading();
        }
        else {
            result = 0;
            humidity = rand() % 100;
            light = rand() % 100;
            temperature = rand() % 80;
        }

        if (moisture < 0) {
            if (spoofData == false) {
                moisture = readMoistureSensor();
            }
            else {
                moisture = rand() % 100;
            }
        }

        //Set date/time using terminal command 'date --set="20180723 18:17"' (year/month/day)
        char timeBuffer[DATE_LEN];
        time_t now = time(NULL);
        struct tm *timeInfo = localtime(&now);
        strftime(timeBuffer, DATE_LEN, "%b %d %H:%M", timeInfo);

        printf("%s [", timeBuffer);
        printf("Temp: %.2f C, Humidity: %.2f%%, ", temperature, humidity);
        printf("Moisture: %.2f%%, ", moisture);
        printf("Light: %.2f%%", light);
        printf("]\n");

        if (result != 0 || light < 0) {
            continue;
        }

        storeLightData(light);
        analyzeBreadboardComponents();


        if (quitApplication == true) {
            break;
        }

        sleep(SLEEP_TIME_READ);
    }
}

void analyzeMoisture() {
    if (quitApplication == false) {
        if (spoofData == false) {
            moisture = readMoistureSensor();

            if (moisture < getWaterThresholdLower()) {
                closeSwitch();
                isWatering = true;
                sleep(10);
            }
            else if (moisture > getWaterThresholdLower()) {
                openSwitch();
                isWatering = false;
            }
            
            if (waterPlant == true && isWatering == false) {
                waterPlant = false;
                closeSwitch();
                sleep(waterLength);
                openSwitch();
            }
        }
        else {
            moisture = rand() % 100;
        }
    }
    else {
        if (spoofData == false) {
            openSwitch();
        }
        printClosingMessage();
    }
}

void triggerPlantWater(int length) {
    waterPlant = true;
    waterLength = length;
}

float getTemperature() {
    return temperature;
}

float getHumidity() {
    return humidity;
}

double getLight() {
    return light;
}

float getMoisture() {
    return moisture;
}

void quitPlantManager() {
    quitApplication = true;
    printf("Closing plant manager");
}

int main(int argc, char *argv[]) {
    initComponents();

    collectData();

    closeComponents();
    printf("\n");
}