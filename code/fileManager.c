#include "fileManager.h"

#define I2C_BUFFER_SIZE 2

/////////////////////
// Global Functions
/////////////////////

FILE *openFile(char filename[], char mode[]) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        printf("Error: could not open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

void closeFile(FILE* file) {
    if (file != NULL) {
        fclose(file);
    }
}

char *readFile(char filename[], char buffer[], int bufferLength) {
    FILE *file = openFile(filename, "r");
    fgets(buffer, bufferLength, file);
    closeFile(file);
    return buffer;
}

int readIntFromFile(char filename[], int bufferLength) {
    char buffer[bufferLength];
    readFile(filename, buffer, bufferLength);
    return atoi(buffer);
}

int writeToFile(char filename[], char value[]) {
    FILE *file = openFile(filename, "w");
    int writeResult = fprintf(file,  "%s",  value);
    if (writeResult <= 0) {
        printf("Error: could not write to file '%s'\n", filename);
        return 1;
    }
    closeFile(file);
    return 0;
}

int initI2CBus(char bus[], int addr) {
    int I2CFile = open(bus, O_RDWR);
    if (I2CFile < 0) {
        printf("Errpr:  Could not open I2C bus '%s' for read/write\n", bus);
        exit(EXIT_FAILURE);
    }
    int result = ioctl(I2CFile, I2C_SLAVE, addr);
    if (result < 0) {
        printf("Error:  Unable to set I2C device to slave address\n");
        exit(EXIT_FAILURE);
    }
    return I2CFile;
}

void writeI2C(int I2CFile, unsigned char regAddr, unsigned char  value) {
    unsigned  char buffer[I2C_BUFFER_SIZE];
    buffer[0] = regAddr;
    buffer[1] = value;
    int result = write(I2CFile, buffer, I2C_BUFFER_SIZE);
    if (result != I2C_BUFFER_SIZE) {
        printf("Error: Unable to write to I2C register. Expected %d, but received %d\n", I2C_BUFFER_SIZE, result);
        exit(EXIT_FAILURE);
    }
}

char *readI2C(int I2CFile, unsigned char regAddr, char value[], int size) {
    int result = write(I2CFile, &regAddr, sizeof(regAddr));
    if (result != sizeof(regAddr)) {
        printf("Error: Unable to write to register during read. Expected %d, but received %d\n", sizeof(regAddr), result);
        exit(EXIT_FAILURE);
    }

    result = read(I2CFile, value, size);
    if (result != size) {
        printf("Error: Unable to read from I2C register. Expected %d, but received %d\n", size, result);
        exit(EXIT_FAILURE);
    }
    return value;
}

void closeI2CBus(int I2CFile) {
    close(I2CFile);
}