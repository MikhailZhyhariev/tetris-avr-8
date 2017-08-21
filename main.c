#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "max7219/max7219.h"
#include "main.h"

// инициализируем блоки
// Y = 1, из-за того, что адрес первого знака (Digit 0) = 0x01
// width, height, X, Y, view
blocks I = {4, 1, 0, 1, {0x0F}};       // I
blocks J = {3, 2, 0, 1, {0x01, 0x07}}; // J
blocks L = {3, 2, 0, 1, {0x04, 0x07}}; // L
blocks O = {2, 2, 0, 1, {0x03, 0x03}}; // O
blocks S = {3, 2, 0, 1, {0x06, 0x03}}; // S
blocks T = {3, 2, 0, 1, {0x02, 0x07}}; // T
blocks Z = {3, 2, 0, 1, {0x03, 0x06}}; // Z

blocks *elements[] = {&I, &J, &L, &O, &S, &T, &Z}; // собираем блоки в массив

int main(void) {
    MAX_Init();

    unsigned char field[FIELD_HEIGHT];
    memset(field, 0x00, FIELD_HEIGHT); // задаем начальные значения для массива field

    // рисуем первый блок
    Block_Draw(&O, field);
    MAX_WriteAllDigits(field);

    while (1) {
        _delay_ms(250);
        Block_Move(&O, field);
    }
}

// Отрисовываем блок путем сложением (побитовым) с соответствующим значением в массиве field
void Block_Draw(blocks *block, unsigned char *field) {
    unsigned char count = 0;
    for (unsigned char i = block->Y; i < block->Y + block->height; i++) {
        field[i - 1] |= block->view[count++] << block->X;
    }
}

// Двигает блок по полю
void Block_Move(blocks *block, unsigned char *field) {
    unsigned char block_end = block->Y + block->height;

    // если блок не дошел до конца поля, то он смещается по оси Y
    // иначе
    if (block_end <= FIELD_HEIGHT) {

        // очищаем предыдущее расположение блока
        unsigned char count = 0;
        for (unsigned char i = block->Y; i < block_end; i++) {
            field[i - 1] ^= block->view[count++] << block->X;
        }

        block->Y++; // смещаем блок по оси Y
    } else {

        block->Y = 1; // перемещаем блок в начало поля

        // смещаем блок по оси X
        // если блок по помещается, то устанавливаем начальное значение X = 0
        if (block->X <= FIELD_WIDTH - 2 * block->width) block->X += block->width;
        else block->X = 0;
    }

    // отрисовываем смещенный блок и перерисовываем поле
    Block_Draw(block, field);
    MAX_WriteAllDigits(field);
}
