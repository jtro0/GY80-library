//
// Created by jtroo on 5-6-19.
//

#ifndef TEL_L3G4200D_H
#define TEL_L3G4200D_H

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "unistd.h"

#include <cstdint>

#define L3G4200D_DEFAULT_ADDR    0x69

#define CTRL_REG1   0x20
#define CTRL_REG2   0x21
#define CTRL_REG3   0x22
#define CTRL_REG4   0x23
#define CTRL_REG5   0x24

#define GYRO_X_L 0x28
#define GYRO_X_H 0x29
#define GYRO_Y_L 0x2A
#define GYRO_Y_H 0x2B
#define GYRO_Z_L 0x2C
#define GYRO_Z_H 0x2D

#define FS_245  245
#define FS_500  500
#define FS_2000 2000

#define FS_245_SELECTION 0b00
#define FS_500_SELECTION 0b01
#define FS_2000_SELECTION 0b11

#define SENSITIVITY_245 0.00875f
#define SENSITIVITY_500 0.0175f
#define SENSITIVITY_2000 0.7f

#define DPS_RPS 0.0174532925199f
#include "chrono"


class l3g4200d {

private:
    uint8_t addr;
    int fd;
    char *bus;
    int selection;

    char config[2] = {0};
    char reg[1] = {0};
    char data[6] = {0};

    float gx;
    float gy;
    float gz;

    float xOffset;
    float yOffset;
    float zOffset;

    std::chrono::high_resolution_clock::time_point timeLast;

    uint8_t range;


public:
    l3g4200d();

    void init();
    void update(bool scaled);

    float getX() { return gx; }

    float getY() { return gy; }

    float getZ() { return gz; }

    std::chrono::high_resolution_clock ::time_point lastUpdate() { return timeLast; }

    void setOffsets(float, float, float);
};



#endif //TEL_L3G4200D_H
