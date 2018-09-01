#include <stdio.h>
#include "dataStore.h"
#include "configuration.h"

#define MINS_IN_DAY 1440
#define REQ_MINUTES_OF_LIGHT 180 //3 hours

static double lightValues[MINS_IN_DAY] = {-1};

static int numValuesSaved = 0;

//Store an array of all the values (except moisture) from the past day (and a bit)
void storeLightData(double light) {
    for (int i = MINS_IN_DAY - 1; i > 0; i--) {
        lightValues[i] = lightValues[i - 1];
    }

    lightValues[0] = light;

    if (numValuesSaved <= MINS_IN_DAY) {
        numValuesSaved++;
    }
}

//Only start to analyze the data if there is a day's worth of data
//Returns true if there isn't enough light over the last ~24 hours
bool analyzeLightData() {
    if (numValuesSaved >= MINS_IN_DAY) {
        int minutesOfLight = 0;
        int expectedLight = getExpectedLight();
        for (int i = 0; i < MINS_IN_DAY; i++) {
            int lightValue = lightValues[i];
            if (lightValue >= expectedLight - LIGHT_ACCURACY) { //&& lightValue <= expectedLight + LIGHT_ACCURACY) { <-- If you want to upper bound temperature
                minutesOfLight++;
            }
        }

        if (minutesOfLight < REQ_MINUTES_OF_LIGHT) {
            return true;
        }
    }
    return false;
}