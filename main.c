#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include "max7219/max7219.h"
#include "block/block.h"
#include "main.h"

int main(void) {
    MAX_Init();

    Random();

    unsigned char field[FIELD_HEIGHT];
    memset(field, 0x00, FIELD_HEIGHT); // задаем начальные значения для массива field

    // инициализируем первый блок
    blocks *block = (blocks*)malloc(sizeof(blocks));
    block = Block_Init();

    // рисуем первый блок
    Block_Add(block, field);
    MAX_WriteAllDigits(field);
    while (1) {
        _delay_ms(250);
        block = Block_Move(block, field);
    }
}

uint8_t EEMEM srnd = 0;
void Random(void) {
    srand(eeprom_read_byte(&srnd));
    if (eeprom_read_byte(&srnd) > 8)
        eeprom_write_byte(&srnd,0);
    else
        eeprom_write_byte(&srnd,(eeprom_read_byte(&srnd)+1));
}
