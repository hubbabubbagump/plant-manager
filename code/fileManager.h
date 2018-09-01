#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define SLOTS "/sys/devices/platform/bone_capemgr/slots"
#define EXPORT "/sys/class/gpio/export"
#define READ "r"
#define WRITE "w"
#define RW "r+"

/**
 * Opens the file pointed to by the argument <filename>. The file is opened in either read or write mode, depending
 * on the argument passed into the <mode> argument.
 * <mode> options:
 *  "r" = read
 *  "w" = write
 *
 */
FILE *openFile(char filename[], char mode[]);

/**
 * Closes the file pointed to by the argument <filename>.
 */
void closeFile(FILE* file);

/**
 * Reads from the file pointed to by the argument <filename>. The function places the results from the read
 * into the character array <buffer>, which has the length <bufferLength>
 */
char *readFile(char filename[], char buffer[], int bufferLength);

/**
 * Reads int from a file if it exists
 */
int readIntFromFile(char filename[], int bufferLength);

/**
 * Writes to the file pointed to by the argument <filename>. The item written to the file is the character array <value>
 */
int writeToFile(char filename[], char value[]);

/**
 * Inits the I2C  bus at '/dev/i2c-#' with the address '0x##'
 */
int initI2CBus(char bus[], int addr);

/**
 * Writes a value '0x##' to a register addr '0x##' in the file returned from initI2CBus
 */
void writeI2C(int I2CFile, unsigned char regAddr, unsigned char  value);

/**
 * Reads a  register address '0x##' from the file returned  from initI2CBus
 * value is the array written to (size of array = # of registers read)
 * size is the size of the array
 */
char *readI2C(int I2CFile, unsigned char regAddr, char value[], int size);

/**
 * Closes an I2C  bus
 */
void closeI2CBus(int I2CFile);

#endif