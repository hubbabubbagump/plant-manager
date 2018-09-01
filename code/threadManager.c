#include <unistd.h>
#include <pthread.h>
#include "threadManager.h"
#include "networkManager.h"
#include "plant-manager.h"

static pthread_t networkTID;
static pthread_t moistureTID;

static bool exitThread = false;

static void *networkThread() {
    initSocket();
    while (!exitThread) {
        listenSocket();
    }
    pthread_exit(NULL);
}

static void *moistureThread() {
    while(!exitThread) {
        analyzeMoisture();
        sleep(SLEEP_TIME_MOISTURE);
    }
    pthread_exit(NULL);
}

void initThreads() {
    pthread_create(&networkTID, NULL, &networkThread, NULL);
    pthread_create(&moistureTID, NULL, &moistureThread, NULL);
}

void exitThreads() {
    pthread_join(networkTID, NULL);
    pthread_join(moistureTID, NULL);
}

void cleanThreads() {
    exitSocket();
    exitThread = true;
}