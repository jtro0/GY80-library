//
// Created by jtroo on 5-6-19.
//

#include "hmc5883l.h"
#include "bitset"
#include "iostream"
hmc5883l::hmc5883l() {
    addr = HMC5883L_DEFAULT_ADDR;
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

void hmc5883l::init() {

    // Wake up device
    config[0] = CONFIG_REGA;
    config[1] = 0x70;
    write(fd, config, 2);

    config[0] = CONFIG_REGB;
    gain = GAIN_1_3;
    config[1] = (char)gain;
    write(fd, config, 2);

    config[0] = MODE_REG;
    config[1] = 0x00;
    write(fd, config, 2);

}


void hmc5883l::update(bool scaled) {
//
//    for (int i=0; i<6; i++) {
//        reg[0] = MAGNO_X_H + i;
//        write(fd, reg, 1);
//        if (read(fd, &data[i], 1) != 1) {
//            printf("Error reading data.");
//            exit(1);
//        }
//    }

    reg[0] = MAGNO_X_H;
    write(fd, reg, 1);

    if (read(fd, data, 6) != 6) {
        printf("Error reading data.");
        exit(1);
    }
    else {

        // Convert the data
        int16_t mx_16 = ((((int)data[0])) <<8 ) | data[1];

        int16_t mz_16 = ((((int)data[2]) <<8 )) |  data[3];

        int16_t my_16 = ((((int)data[4]) <<8 )) |  data[5];
//        std::bitset<8> x1(data[0]);
//        std::bitset<8> x2(data[1]);
//        std::bitset<8> y1(data[2]);
//        std::bitset<8> y2(data[3]);
//        std::bitset<8> z1(data[4]);
//        std::bitset<8> z2(data[5]);
//
//        std::cout << x1 << ' ' << x2 << '\n' << y1 << ' ' << y2 << '\n' << z1 << ' ' << z2 << '\n';

        if (scaled) {

            switch (gain) {
                case GAIN_1_3:
                    LSB_XY = 1100;
                    LSB_Z = 980;
                    break;
                case GAIN_1_9:
                    LSB_XY = 855;
                    LSB_Z = 760;
            }
            mx = ((float)mx_16 / LSB_XY) + xOffset;
            my = ((float)my_16 / LSB_XY) + yOffset;
            mz = ((float)mz_16 / LSB_Z) + zOffset;
        } else {
            mx = (float)mx_16;
            my = (float)my_16;
            mz = (float)mz_16;
        }
//        printf("Mx: %0.2f    My: %0.2f    Mz: %0.2f\n", mx, my, mz);

    }
}

float hmc5883l::getPitch() {
    return (atan2(mx, sqrt(my*my + mz*mz)) * 180.0) / M_PI;
}
float hmc5883l::getRoll() {
    return (atan2(my, sqrt(mx*mx + mz*mz)) * 180.0) / M_PI;
}

float hmc5883l::getYaw() {
    return (atan2(mz, sqrt(mx*mx + my*my)) * 180.0) / M_PI;

}


void hmc5883l::setOffsets(float x, float y, float z) {
    xOffset = x;
    yOffset = y;
    zOffset = z;
}