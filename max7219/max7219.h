#ifndef MAX7219_H
#define MAX7219_H

#define DECODE_MODE 0x00 // no decode
#define INTENSITY 0x0F // set min intensity
#define SCAN_LIMIT 0x07 // set scan digit (0-7)
#define SHUTDOWN 0x01 // set normal mode
#define DISPLAY_TEST 0x00 // display test off

void MAX_Init();

void MAX_WriteDigit(unsigned char address, unsigned char data);

void MAX_WriteAllDigits(unsigned char *data);

void MAX_ClearDigit(unsigned char address);

void MAX_ClearAllDigits();


#endif
