#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// WHEN ADDING A CONSTANT, UPDATE FUNCTIONS saveConfig(), setToDefault(), AND setValue().
// ALSO ADD A GETTER AND/OR SETTER IF NEEDED

//WATER AND MOISTURE CONSTANTS
#define WATER_THRESHOLD_UPPER "water_thresh_upper"
#define WATER_THRESHOLD_UPPER_DEFAULT 80
#define WATER_THRESHOLD_LOWER "water_thresh_lower"
#define WATER_THRESHOLD_LOWER_DEFAULT 50

//general limits for potted houseplants 
//note RHT03 has accuracy of  +/-2%(Max +/- 5%) for Humidity
// and +/- 0.5 celsius (from datasheet) 
//TEMPERATURE CONSTANTS
#define TEMPERATURE_THRESHOLD_UPPER "temp_thresh_upper"
#define TEMPERATURE_THRESHOLD_UPPER_DEFAULT 25
#define TEMPERATURE_THRESHOLD_LOWER "temp_thresh_lower"
#define TEMPERATURE_THRESHOLD_LOWER_DEFAULT 15

//HUMIDITY CONSTANTS
#define HUMIDITY_THRESHOLD_UPPER "humidity_thresh_upper"
#define HUMIDITY_THRESHOLD_UPPER_DEFAULT 60
#define HUMIDITY_THRESHOLD_LOWER "humidity_thresh_lower"
#define HUMIDITY_THRESHOLD_LOWER_DEFAULT 40

//LIGHT CONSTANTS
#define LIGHT_EXPECTED "light_expected"
#define LIGHT_EXPECTED_DEFAULT 80

#define DELIMITER "="
#define CONFIG_FILENAME "configuration.ini"
#define BUFFER_SIZE 512

static int water_threshold_lower;
static int water_threshold_upper;
static int temp_threshold_lower;
static int temp_threshold_upper;
static int humidity_threshold_lower;
static int humidity_threshold_upper;
static int light_expected;

static bool isDirty;

static void saveConfig() {
    FILE *configFile = fopen(CONFIG_FILENAME, "wb");
        if (configFile != NULL) {
            //print all config values to file using format "<value name>=<value>"
            fprintf(configFile, "%s=%d\n", WATER_THRESHOLD_LOWER, water_threshold_lower);
            fprintf(configFile, "%s=%d\n", WATER_THRESHOLD_UPPER, water_threshold_upper);
            fprintf(configFile, "%s=%d\n", TEMPERATURE_THRESHOLD_LOWER, temp_threshold_lower);
            fprintf(configFile, "%s=%d\n", TEMPERATURE_THRESHOLD_UPPER, temp_threshold_upper);
            fprintf(configFile, "%s=%d\n", HUMIDITY_THRESHOLD_LOWER, humidity_threshold_lower);
            fprintf(configFile, "%s=%d\n", HUMIDITY_THRESHOLD_UPPER, humidity_threshold_upper);
            fprintf(configFile, "%s=%d\n", LIGHT_EXPECTED, light_expected);
            fclose(configFile);
        }
        else {
            printf("Error: Unable to create configuration file\n");
            exit(EXIT_FAILURE);
        }
}

static void setToDefault() {
    water_threshold_lower = WATER_THRESHOLD_LOWER_DEFAULT;
    water_threshold_upper = WATER_THRESHOLD_UPPER_DEFAULT;
    temp_threshold_lower = TEMPERATURE_THRESHOLD_LOWER_DEFAULT;
    temp_threshold_upper = TEMPERATURE_THRESHOLD_UPPER_DEFAULT;
    humidity_threshold_lower = HUMIDITY_THRESHOLD_LOWER_DEFAULT;
    humidity_threshold_upper = HUMIDITY_THRESHOLD_UPPER_DEFAULT;
    light_expected = LIGHT_EXPECTED_DEFAULT;
}

static void setValue(char *token, char *value) {
    if (strcmp(token, WATER_THRESHOLD_UPPER) == 0) {
        water_threshold_upper = atoi(value);
    }
    else if (strcmp(token, WATER_THRESHOLD_LOWER) == 0) {
        water_threshold_lower = atoi(value);
    }
    else if (strcmp(token, TEMPERATURE_THRESHOLD_UPPER) == 0) {
        temp_threshold_upper = atoi(value);
    }
    else if (strcmp(token, TEMPERATURE_THRESHOLD_LOWER) == 0) {
        temp_threshold_lower = atoi(value);
    }
    else if (strcmp(token, HUMIDITY_THRESHOLD_UPPER) == 0) {
        humidity_threshold_upper = atoi(value);
    }
    else if (strcmp(token, HUMIDITY_THRESHOLD_LOWER) == 0) {
        humidity_threshold_lower = atoi(value);
    }
    else if (strcmp(token, LIGHT_EXPECTED) == 0) {
        light_expected = atoi(value);
    }
}

static void readFile(FILE *file) {
    //read values from file as set to variables. use strtok
    char buffer[BUFFER_SIZE];

    while(fgets(buffer, sizeof(buffer), file)) {
        char *token = strtok(buffer, DELIMITER);
        if (token != NULL) {
            char *value = strtok(NULL, "\n");
            if (value != NULL) {
                setValue(token, value);
            }
        }
    }
}

/*********************************
 * GETTERS AND SETTERS
 ********************************/

int getWaterThresholdUpper() {
    return water_threshold_upper;
}

void setWaterThresholdUpper(int value) {
    // if (value <= water_threshold_lower) {
    //     printf("Error: Unable to set upper limit for water lower than the lower limit\n");
    //     return;
    // }
    // else 
    if (value < 0 || value > 100) {
        printf("Error: Invalid value\n");
        return;
    }

    if (water_threshold_upper != value) {
        water_threshold_upper = value;
        isDirty = true;
    }
}

int getWaterThresholdLower() {
    return water_threshold_lower;
}

void setWaterThresholdLower(int value) {
    // if (value >= water_threshold_upper) {
    //     printf("Error: Unable to  set lower limit for water higher than the upper limit\n");
    //     return;
    // }
    // else 
    if (value < 0 || value > 100) {
        printf("Error: Invalid value\n");
        return;
    }

    if (water_threshold_lower != value) {
        water_threshold_lower = value;
        isDirty = true;
    }
}

int getTempThresholdUpper() {
    return temp_threshold_upper;
}

void setTempThresholdUpper(int value) {
    // if (value <= temp_threshold_lower) {
    //     printf("Error:  Unable to set upper limit for temperature lower than the lower limit\n");
    //     return;
    // }

    if (temp_threshold_upper != value) {
        temp_threshold_upper = value;
        isDirty = true;
    }
}

int getTempThresholdLower() {
    return temp_threshold_lower;
}

void setTempThresholdLower(int value) {
    // if (value >= temp_threshold_upper) {
    //     printf("Error: Unable to set lower limit for  temperature  higher than the upper limit\n");
    //     return;
    // }

    if (temp_threshold_lower != value) {
        temp_threshold_lower = value;
        isDirty = true;
    }
}

int getHumidityThresholdUpper() {
    return humidity_threshold_upper;
}

void setHumidityThresholdUpper(int value) {
    // if (value <= humidity_threshold_lower) {
    //     printf("Error: Unable to set upper limit for humidity lower than the lower limit\n");
    //     return;
    // }
    // else 
    if (value < 0 || value > 100) {
        printf("Error: Invalid value\n");
        return;
    }

    if (humidity_threshold_upper != value) {
        humidity_threshold_upper = value;
        isDirty = true;
    }
}

int getHumidityThresholdLower() {
    return humidity_threshold_lower;
}

void setHumidityThresholdLower(int value) {
    // if (value >= humidity_threshold_upper) {
    //     printf("Error: Unable to set lower limit for humidity  higher than the upper limit\n");
    //     return;
    // }
    // else 
    if (value < 0 || value > 100) {
        printf("Error: Invalid value\n");
        return;
    }

    if (humidity_threshold_lower != value) {
        humidity_threshold_lower = value;
        isDirty = true;
    }
}

int getExpectedLight() {
    return light_expected;
}

void setExpectedLight(int value) {
    if (value < 0 || value > 100) {
        printf("Error: Invalid value\n");
        return;
    }

    if (light_expected != value) {
        light_expected = value;
        isDirty = true;
    }
}

/*********************************
 * GLOBAL FUNCTIONS
 ********************************/

void checkDirty() {
    if (isDirty) {
        saveConfig();
        isDirty = false;
    }
}

void initConfig() {
    isDirty = false;
    setToDefault();

    FILE *configurationFile = fopen(CONFIG_FILENAME, "rb+");
    if (configurationFile != NULL) {
        readFile(configurationFile);
        fclose(configurationFile);

        //Overwrite with values, in case we add new constants to save
        saveConfig();
        return;
    }
    
    //Save default values into a new config file
    saveConfig();
}