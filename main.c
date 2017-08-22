#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "max7219/max7219.h"
#include "block/block.h"
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
    Block_Add(elements[3], field);
    MAX_WriteAllDigits(field);

    while (1) {
        _delay_ms(250);
        Block_Move(elements[3], field);
    }
}
