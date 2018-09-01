#include <stdio.h>
#include <termios.h>
#include <stdbool.h>
#include "fileManager.h"
#include "pins.h"
#include "plant-manager.h"

//RELAY
#define RELAY_ACTIVE_LOW "/sys/class/gpio/gpio2/active_low"
#define RELAY_DIRECTION "/sys/class/gpio/gpio2/direction"
#define RELAY_VALUE "/sys/class/gpio/gpio2/value"
#define ACTIVE_LOW 0
#define DIRECTION "out"

//MOISTURE SENSOR
#define I2C2 "BB-I2C2"
#define DEV_I2C2 "/dev/i2c-2"
#define MOISTURE_SENSOR_ADDR 0x50
#define MOISTURE_SENSOR_ADDR_CONFIG 0x02
#define STARTING_REG 0x00
#define NUM_REG 2
#define BYTE_SIZE 8
#define SENSOR_MAX_DEFAULT 1462
#define NORMALIZED_MAX 100.0

static int moistureSensor = -1;
static int sensor_max;

static bool isSwitchOpen = false;

//opens the switch in the relay
void openSwitch() {
    if (isSwitchOpen == false) {
        writeToFile(RELAY_VALUE, "0");
        isSwitchOpen = true;
    }
}

//closes the switch in the relay
void closeSwitch() {
    if (isSwitchOpen == true) {
        writeToFile(RELAY_VALUE, "1");
        isSwitchOpen = false;
    }
}

//Returns a value between 0 and 100
float readMoistureSensor() {
    if (moistureSensor >= 0) {
        char data[NUM_REG];
        readI2C(moistureSensor, STARTING_REG, data, NUM_REG);

        int value = ((data[0] & 0x0f) << BYTE_SIZE | (data[1])) & 0xfff;
        if (value > sensor_max) {
            sensor_max = value;
        }

        float normalizedValue = value * NORMALIZED_MAX / sensor_max;
        return normalizedValue;
    }
    else {
        printf("Error: moisture sensor not opened\n");
        return -1;
    }
}

void initializeGrove() {
    if (spoofSensorData() == false) {
        //Init relay
        writeToFile(EXPORT, P9_22);
        writeToFile(RELAY_ACTIVE_LOW, ACTIVE_LOW);
        writeToFile(RELAY_DIRECTION, DIRECTION);
        openSwitch(); //Ensure that the switch is open

        //Init moisture sensor
        sensor_max = SENSOR_MAX_DEFAULT;
        writeToFile(SLOTS, I2C2);
        moistureSensor = initI2CBus(DEV_I2C2, MOISTURE_SENSOR_ADDR);
        writeI2C(moistureSensor, MOISTURE_SENSOR_ADDR_CONFIG, 0x20);
    }
}

void closeGrove() {
    if (spoofSensorData() == false) {
        openSwitch();
        closeI2CBus(moistureSensor);
    }
}