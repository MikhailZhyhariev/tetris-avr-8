#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include "max7219/max7219.h"
#include "block/block.h"
#include "main.h"

// инициализируем первый блок
blocks *block;
// инициализируем массив-поле
unsigned char field[FIELD_HEIGHT];

int main(void) {
    MAX_Init();

    Random();

    // настройка внешний прерываний, для управления блоком
    EICRA = 0x00; // внешние прерывания происходят при низком уровне на соотв. выводе
    EIMSK |= (1 << INT0)|(1 << INT1)|(1 << INT2); // разрешаем локальные прерывания

    // настройка таймера Timer 1
    // режим Fast PWM, предделитель на 64 (1 000 000 / 64 = 15625 тиков/c)
    TCCR1B |= (1 << ICES1)|(1 << CS11)|(1 << CS10)|(1 << WGM12)|(1 << WGM13);
    TCCR1A |= (1 << WGM11);
    TIMSK  |= (1 << OCIE1A); // разрешаем локальное прерывание по сравнению таймера
    ICR1 = 7500; // устанавливаем значение регистра захвата-сравнения
    TCNT1H = 0;
    TCNT1L = 1; // сбрасываем верхний и нижний байты счетчика таймера

    // задаем начальные значения для массива field
    memset(field, 0x00, FIELD_HEIGHT);

    // выделяем память для блока
    block = (blocks*)malloc(sizeof(blocks));
    block = Block_Init();

    // рисуем первый блок
    Block_Add(block, field);
    MAX_WriteAllDigits(field);

    sei(); // разрешаем прерывания глобально
    while (1);
}

ISR(TIMER1_COMPA_vect) {
    cli(); // запрещаем прерывания
    block = Block_MoveDown(block, field); // двигаем блок
    sei(); // разрешаем прерывания
}

ISR(INT0_vect) {
    Block_MoveLeft(block, field);
}

ISR(INT1_vect) {
    Block_MoveRight(block, field);
}

ISR(INT2_vect) {
    cli();
    Block_Transform(block, field);
    sei();
}

uint8_t EEMEM srnd = 0;
void Random(void) {
    srand(eeprom_read_byte(&srnd));
    if (eeprom_read_byte(&srnd) > 8)
        eeprom_write_byte(&srnd,0);
    else
        eeprom_write_byte(&srnd,(eeprom_read_byte(&srnd)+1));
}


void Swap(unsigned char *a, unsigned char *b) {
    unsigned char temp = *a;
    *a = *b;
    *b = temp;
}
