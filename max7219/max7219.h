#ifndef MAX7219_H
#define MAX7219_H

#define DECODE_MODE 0x00 // НЕ декодируем значения (Digit) как семисегментные
#define INTENSITY 0x0F // задаем максимальную яркость
#define SCAN_LIMIT 0x07 // сканируем все 8 знаков (Digit)
#define SHUTDOWN 0x01 // ключаем нормальный режим работы
#define DISPLAY_TEST 0x00 // выключаем display test

void MAX_Init();

void MAX_WriteDigit(unsigned char address, unsigned char data);

void MAX_WriteSomeDigits(unsigned char from, unsigned char to, unsigned char *data);

void MAX_WriteAllDigits(unsigned char *data);

void MAX_ClearDigit(unsigned char address);

void MAX_ClearAllDigits();


#endif
