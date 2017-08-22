#include "spi/spi.h"
#include "max7219.h"

// Функция инициализации драйвера
void MAX_Init() {
    // инициализируем МК ведущим
    SPI_MasterInit();

    // 0 - режим декодирования
    // 1 - яркость
    // 2 - предел сканирования
    // 3 - режим работы драйвера
    // 4 - режим тестирования
    unsigned char initRegister[5] = {0x09, 0x0A, 0x0B, 0x0C, 0x0F};
    unsigned char initRegisterValue[5] = {DECODE_MODE, INTENSITY, SCAN_LIMIT, SHUTDOWN, DISPLAY_TEST};

    // настраиваем драйвер
    for (int i = 0; i < 5; i++) {
        MAX_WriteDigit(initRegister[i], initRegisterValue[i]);
    }

    MAX_ClearAllDigits();
}

void MAX_WriteDigit(unsigned char address, unsigned char data) {
    // записывает первый байт (address), содержащий адрес регистра знакоместа (Digit)
    SPI_MasterTransmit(address, 0);
    // записывает второй байт - значение (data), которое отобразится на соотв. знакоместе
    SPI_MasterTransmit(data, 1);
}

void MAX_WriteSomeDigits(unsigned char from, unsigned char to, unsigned char *data) {
    for (unsigned char address = from; address <= to; address++) {
        MAX_WriteDigit(address, data[address - 1]);
    }
}

void MAX_WriteAllDigits(unsigned char *data) {
    for (unsigned char address = 1; address < 9; address++) {
        MAX_WriteDigit(address, data[address - 1]);
    }
}

void MAX_ClearDigit(unsigned char address) {
    MAX_WriteDigit(address, 0x00);
}

void MAX_ClearAllDigits() {
    for (unsigned char i = 1; i < 9; i++) {
      MAX_ClearDigit(i);
    }
}
