#include "networkManager.h"
#include "configuration.h"
#include "plant-manager.h"

#include <ctype.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 12345
#define RECV_MSG_LEN 1024
#define SEND_MSG_LEN 1500

#define STOP_CMD "stop\n"
#define STOP_MSG "Application shutting down"
#define UPDATE_CMD "update\n"
#define UPDATE_SET_CMD "updateSetting"
#define INVALID_MSG "This is an invalid command"
#define WATER_CMD "waterPlant\n"

// In format: updateSetting #light #moist #temp\n
// Ordering of updateSetting
#define LIGHT_VAL 0
#define MOIST_VAL_UPPER 1
#define MOIST_VAL_LOWER 2
#define TEMP_VAL_UPPER 3
#define TEMP_VAL_LOWER 4
#define HUMID_VAL_UPPER 5
#define HUMID_VAL_LOWER 6
#define NUM_SET_PARAMS 7
#define LAST_PARAM HUMID_VAL_LOWER
#define WATER_PLANT_LENGTH 5

static bool validateInt(char *token);
static void selectMessage(char recvMsg[], char sendMsg[]);

static struct sockaddr_in sin;
static bool stop = false;
static int socketDescriptor = -1;
static unsigned int sinLen;
static const char spaceChar[2] = " ";
static const char newlnChar[2] = "\n";

static char* formatMessage(char sendMsg[]) {
    sprintf(sendMsg, "%d,%d,%d,%d,%d,%d,%d,%lf,%f,%f,%f", 
        getExpectedLight(), getWaterThresholdUpper(), getWaterThresholdLower(),
        getTempThresholdUpper(), getTempThresholdLower(), 
        getHumidityThresholdUpper(), getHumidityThresholdLower(), 
        getLight(), getMoisture(), getTemperature(), getHumidity());
    return sendMsg;
}

static bool validateInt(char *token) {
    bool isNumber = true;
    for (int i = 0; i < strlen(token); i++) {
        if (!isdigit(token[i]) && token[i] != '-') {
            isNumber = false;
        }
    }
    return isNumber;
}

// In need of testing
static void selectMessage(char recvMsg[], char sendMsg[]) {
    if (strcmp(recvMsg, STOP_CMD) == 0) {
        stop = true;
        sprintf(sendMsg, "%s", STOP_MSG);
        quitPlantManager();
    } else if (strcmp(recvMsg, WATER_CMD) == 0) {
        triggerPlantWater(WATER_PLANT_LENGTH);
        formatMessage(sendMsg);
    } else if (strcmp(recvMsg, UPDATE_CMD) == 0) {
        formatMessage(sendMsg);
    } else {
        char *token;
        token = strtok(recvMsg, spaceChar);
        if (strcmp(token, UPDATE_SET_CMD) == 0) {
            for (int i = 0; i < NUM_SET_PARAMS; i++) {
                if (i == LAST_PARAM) {
                    token = strtok(NULL, newlnChar);
                } else {
                    token = strtok(NULL, spaceChar);
                }
                bool isNumber = validateInt(token);
                if (isNumber) {
                    if (i == LIGHT_VAL) {
                        setExpectedLight(atoi(token));
                    } else if (i == MOIST_VAL_UPPER) {
                        setWaterThresholdUpper(atoi(token));
                    } else if (i == MOIST_VAL_LOWER) {
                        setWaterThresholdLower(atoi(token));
                    } else if (i == TEMP_VAL_UPPER) {
                        setTempThresholdUpper(atoi(token));
                    } else if (i == TEMP_VAL_LOWER) {
                        setTempThresholdLower(atoi(token));
                    } else if (i == HUMID_VAL_UPPER) {
                        setHumidityThresholdUpper(atoi(token));
                    } else if (i == HUMID_VAL_LOWER) {
                        setHumidityThresholdLower(atoi(token));
                    }
                }
            }
            checkDirty();
            formatMessage(sendMsg);
        } else {
            sprintf(sendMsg, "%s", INVALID_MSG);
        }
    }
}

void listenSocket() {
    if (!stop) {
        char recvMsg[RECV_MSG_LEN];
        char sendMsg[SEND_MSG_LEN];
        memset(recvMsg, 0, sizeof(recvMsg));
        memset(sendMsg, 0, sizeof(sendMsg));
        sinLen = sizeof(sin);
        recvfrom(socketDescriptor, recvMsg, RECV_MSG_LEN, 0, (struct sockaddr*) &sin, &sinLen);
        selectMessage(recvMsg, sendMsg);
        sendto(socketDescriptor, sendMsg, strlen(sendMsg), 0, (struct sockaddr*) &sin, sinLen);
    }
}

void initSocket() {
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
}

void exitSocket() {
    if (socketDescriptor >= 0) {
        close(socketDescriptor);
    }
}