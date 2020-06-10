//
// Created by jtroo on 5-6-19.
//

#ifndef TEL_HMC5883L_H
#define TEL_HMC5883L_H



#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "unistd.h"

#include "math.h"
#include <cstdint>

#define HMC5883L_DEFAULT_ADDR    0x1e

#define CONFIG_REGA 0x00
#define CONFIG_REGB 0x01
#define MODE_REG 0x02

#define MAGNO_X_H   0x03
#define MAGNO_X_L   0x04
#define MAGNO_Z_H   0x05
#define MAGNO_Z_L   0x06
#define MAGNO_Y_H   0x07
#define MAGNO_Y_L   0x08

typedef enum {
    GAIN_0_88   = 0x00,
    GAIN_1_3    = 0x20,
    GAIN_1_9    = 0x40,
    GAIN_2_5    = 0x60,
    GAIN_4_0    = 0x80,
    GAIN_4_7    = 0xA0,
    GAIN_5_6    = 0xC0,
    GAIN_8_1    = 0xE0
} GAIN;

class hmc5883l {

private:
    uint8_t addr;
    int fd;
    char *bus;
    GAIN gain;

    int LSB_XY;
    int LSB_Z;

    char config[2] = {0};
    char reg[1] = {0};
    char data[6] = {0};

    float mx;
    float my;
    float mz;

    float xOffset;
    float yOffset;
    float zOffset;

public:
    hmc5883l();

    void init();
    void update(bool scaled);

    float getX() { return mx; }

    float getY() { return my; }

    float getZ() { return mz; }

    float getPitch();
    float getRoll();
    float getYaw();

    void setOffsets(float, float, float);

};





#endif //TEL_HMC5883L_H
