//
// Created by jtroo on 5-6-19.
//

#include "l3g4200d.h"
#include "bitset"
#include "iostream"
#include <string.h>
l3g4200d::l3g4200d() {
    addr = L3G4200D_DEFAULT_ADDR;
    range = FS_245;
    bus = "/dev/i2c-1";
    selection = FS_245;
    memset(data, 0, 6);

    xOffset = 0;
    yOffset = 0;
    zOffset = 0;

    if ((fd = open(bus, O_RDWR)) < 0) {
        printf("Failed to open the bus.\n");
        exit(1);
    }
    ioctl(fd, I2C_SLAVE, addr);
}

void l3g4200d::init() {

    // Wake up device
    config[0] = CTRL_REG1;
    config[1] = 0x0F;
    write(fd, config, 2);

    config[0] = CTRL_REG4;
    config[1] = 0;
    write(fd, config, 2);
}


void l3g4200d::update(bool scaled) {

    memset(data, 0, 6);

    for (int i=0; i<6; i++) {
        reg[0] = GYRO_X_L + i;

        write(fd, reg, 1);
        if (read(fd, &data[i], 1) != 1) {
            printf("Error reading data.");
            exit(1);
        }
//        std::bitset<8> current(data[i]);
//        std::cout << current << std::endl;
    }

//    reg[0] = GYRO_X_L;
//    write(fd, reg, 1);
//
//    if (read(fd, data, 6) != 6) {
//        printf("Error reading data.");
//        exit(1);
//    }

    // Convert the data
    int16_t gx_16 = ((((int)data[1])) <<8 ) | data[0];
//    if(gx_16 > 0x7FFF)
//    {
//        gx_16 -= 0x10000;
//    }

    int16_t gy_16 = ((((int)data[3]) <<8 )) |  data[2];
//    if(gy_16 > 0x7FFF)
//    {
//        gy_16 -= 0x10000;
//    }

    int16_t gz_16 = ((((int)data[5]) <<8 )) |  data[4];
//    if(gz_16 > 0x7FFF)
//    {
//        gz_16 -= 0x10000;
//    }

    if (scaled) {
        if (selection == FS_245) {
            gx = (gx_16 * SENSITIVITY_245) - xOffset;
            gy = (gy_16 * SENSITIVITY_245) - yOffset;
            gz = (gz_16 * SENSITIVITY_245) - zOffset;
        }
    } else {
        gx = (float)gx_16;
        gy = (float)gy_16;
        gz = (float)gz_16;
    }
    timeLast = std::chrono::high_resolution_clock::now();
//    printf("gx: %0.2f    gy: %0.2f    gz: %0.2f", gx, gy, gz);


}

void l3g4200d::setOffsets(float x, float y, float z) {
    xOffset = x;
    yOffset = y;
    zOffset = z;
}



