//
// Created by jtroo on 5-6-19.
//

#ifndef TEL_ADXL345_H
#define TEL_ADXL345_H

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "unistd.h"

#include <cstdint>
#include "math.h"

//#define scale   0.004f

#define ADXL345_DEFAULT_ADDR    0x53

#define POWER_CTL   0x2D
#define DATA_FORMAT 0x31
#define FIFO    0x38

#define ACC_X_L  0x32
#define ACC_X_H  0x33
#define ACC_Y_L  0x34
#define ACC_Y_H  0x35
#define ACC_Z_L  0x36
#define ACC_Z_H  0x37

#define MEASURE 0x08

#define RANGE_2G    0
#define RANGE_4G    1
#define RANGE_8G    2
#define RANGE_16G    3


class adxl345 {

private:
    uint8_t addr;
    int fd;
    char *bus;

    char config[2] = {0};
    char reg[1] = {0};
    char data[6] = {0};

    float ax;
    float ay;
    float az;

    float xOffset;
    float yOffset;
    float zOffset;

    uint8_t range;


public:
    adxl345();

    void init();
    void update(bool scaled);

    float getX() { return ax; }

    float getY() { return ay; }

    float getZ() { return az; }

    float getPitch();
    float getRoll();
    float getYaw();

    void setOffsets(float, float, float);


};


#endif //TEL_ADXL345_H
