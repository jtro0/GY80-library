//
// Created by jtroo on 5-6-19.
//

#include "adxl345.h"

#include "bitset"
#include "iostream"
adxl345::adxl345() {
    addr = ADXL345_DEFAULT_ADDR;
    range = RANGE_2G;
    bus = "/dev/i2c-1";

    xOffset = 0;
    yOffset = 0;
    zOffset = 0;

    if ((fd = open(bus, O_RDWR)) < 0) {
        printf("Failed to open the bus.\n");
        exit(1);
    }
    ioctl(fd, I2C_SLAVE, addr);
}

void adxl345::init() {

    // Wake up device
    config[0] = POWER_CTL;
    config[1] = MEASURE;
    write(fd, config, 2);

    config[0] = DATA_FORMAT;
    config[1] = 1 << 3 | range;
    write(fd, config, 2);
}

void adxl345::update(bool scaled) {
    reg[0] = ACC_X_L;
    write(fd, reg, 1);

    if (read(fd, data, 6) != 6) {
        printf("Error reading data.");
        exit(1);
    }
    else {
//        int16_t ax_16 = (data[1]<<8) + data[0];
//        if ((ax_16 & (1<<15)) != 0) {
//            ax_16 = ax_16 | ~((1<<16) -1);
//        }
//        else
//            ax_16 = ax_16;
//
//        int16_t ay_16 = (data[3]<<8) + data[2];
//        if ((ay_16 & (1<<15)) != 0) {
//            ay_16 = ay_16 | ~((1<<16) -1);
//        }
//        else
//            ay_16 = ay_16;
//
//        int16_t az_16 = (data[5]<<8) + data[4];
//        if ((az_16 & (1<<15)) != 0) {
//            az_16 = az_16 | ~((1<<16) -1);
//        }
//        else
//            az_16 = az_16;

        int16_t ax_16 = ((((int)data[1])) <<8 ) | data[0];

        int16_t ay_16 = ((((int)data[3]) <<8 )) |  data[2];

        int16_t az_16 = ((((int)data[5]) <<8 )) |  data[4];

//        x = (((int)data[1]) << 8) | data[0];
//        y = (((int)data[3]) << 8) | data[2];
//        z = (((int)data[5]) << 8) | data[4];

        if (scaled) {
            ax = (ax_16 * 0.0039) - xOffset;
            ay = (ay_16 * 0.0039) - yOffset;
            az = (az_16 * 0.0039) - zOffset;

        }
        else {
            ax = (float)ax_16;
            ay = (float)ay_16;
            az = (float)az_16;
        }

//        printf("Ax: %0.2f    Ay: %0.2f    Az: %0.2f\n", ax, ay, az);
    }

}

float adxl345::getPitch() {
    return (atan2(ax, sqrt(ay*ay + az*az)) * 180.0) / M_PI;
}
float adxl345::getRoll() {
    return (atan2(ay, sqrt(ax*ax + az*az)) * 180.0) / M_PI;
}

float adxl345::getYaw() {
    return (atan2(az, sqrt(ax*ax + ay*ay)) * 180.0) / M_PI;

}


void adxl345::setOffsets(float x, float y, float z) {
    xOffset = x;
    yOffset = y;
    zOffset = z;
}