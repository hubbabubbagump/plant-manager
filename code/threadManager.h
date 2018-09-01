// Handles all threads we use in the application
#ifndef _THREADHANDLER_H_
#define _THREADHANDLER_H_

#include <pthread.h>
#include <stdbool.h>

void initThreads(void);
void exitThreads(void);
void cleanThreads(void);

#endif