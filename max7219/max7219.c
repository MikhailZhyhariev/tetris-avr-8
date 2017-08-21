#include <math.h>
#include "../spi/spi.h"
#include "max7219.h"

void MAX_Init() {
    SPI_MasterInit();

    // 0 - decode mode
    // 1 - intensity
    // 2 - scan limit
    // 3 - shutdown mode
    // 4 - display test mode
    unsigned char initRegister[5] = {0x09, 0x0A, 0x0B, 0x0C, 0x0F};
    unsigned char initRegisterValue[5] = {DECODE_MODE, INTENSITY, SCAN_LIMIT, SHUTDOWN, DISPLAY_TEST};

    // configure driver
    for (int i = 0; i < 5; i++) {
        MAX_WriteDigit(initRegister[i], initRegisterValue[i]);
    }

    MAX_ClearAllDigits();
}

void MAX_WriteDigit(unsigned char address, unsigned char data) {
    SPI_MasterTransmit(address, 0);
    SPI_MasterTransmit(data, 1);
}

void MAX_WriteAllDigits(unsigned char *data) {
    for (int address = 1; address < 9; address++) {
        MAX_WriteDigit(address, data[address - 1]);
    }
}

void MAX_ClearDigit(unsigned char address) {
    MAX_WriteDigit(address, 0x00);
}

void MAX_ClearAllDigits() {
    for (int i = 1; i < 9; i++) {
      MAX_ClearDigit(i);
    }
}
