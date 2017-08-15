#include <avr/io.h>
#include "max7219/max7219.h"

int main(void) {
    MAX_Init();

    unsigned char digitsState[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

    MAX_WriteAllDigits(digitsState);
    while (1) {
    }
}
