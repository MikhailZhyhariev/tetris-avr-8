#include <string.h>
#include <stdlib.h>
#include "../main.h"
#include "../max7219/max7219.h"
#include "block.h"

// Добавляем блок на поле сложением (побитовым) с соответствующим значением в массиве field
void Block_Add(blocks *block, unsigned char *field) {
    unsigned char count = 0;
    for (unsigned char i = block->Y; i < block->Y + block->height; i++) {
        field[i - 1] |= block->view[count++] << block->X;
    }
}

// Двигает блок по полю
void Block_Move(blocks *block, unsigned char *field) {
    unsigned char block_end = block->Y + block->height; // конец блока

    // если блок не дошел до конца поля и значение, которое вернула функция
    // проверки коллизии равно высоте блока, то он смещается по оси Y
    // иначе
    if ((block_end <= FIELD_HEIGHT) & (Block_Collision(block, field) == block->height)) {

        // очищаем предыдущее расположение блока
        unsigned char count = 0; // индекс массива block.view
        for (unsigned char i = block->Y; i < block_end; i++) {
            field[i - 1] ^= block->view[count++] << block->X;
        }

        block->Y++; // смещаем блок по оси Y
    } else {

        block->Y = 1; // перемещаем блок в начало поля

        // смещаем блок по оси X
        // если блок не помещается, то устанавливаем начальное значение X = 0
        if (block->X <= FIELD_WIDTH - 2 * block->width) block->X += block->width;
        else block->X = 0;

        // if (field[0] > 0x11) memset(field, 0x00, FIELD_HEIGHT);
    }

    // отрисовываем смещенный блок и перерисовываем поле
    Block_Add(block, field);
    MAX_WriteAllDigits(field);
}

// Проверяет коллизию блока и поля. Возвращает результат поэлементного
// сложения по модулю 2 (XOR) массива block.view и элемента поля, который находится
// ниже соотв. элемента массива block.view
unsigned char Block_Collision(blocks *block, unsigned char *field) {
    unsigned char block_end = block->Y + block->height; // конец блока
    unsigned char count = 0; // индекс массива block.view
    unsigned char result = 0;

    /* для каждого элемента массива block.view проверяем возможность коллизии
    с элементом поля, который находится ниже элемента block.view. Если на элементе
    поля находится часть блока, то сначала из вычитаем эту часть блока из поля. Получаем нужный участок поля (который соответствует расположению блока по оси X)
    путем побитового умножения элемента массива block.view с нужным элементом поля
    после этого складываем по модулю 2 (XOR) полученный участок поля и
    элемент массива block.view. Если результат XOR больше или равен значению соотв.
    элемента block.view, то инкрементируем переменную result (потому что при сдвиге блока не возникнет коллизии). */
    for (unsigned char i = block->Y; i < block_end; i++) {

        // значение текущей части блока
        unsigned char block_value = block->view[count++] << block->X;

        // значение следующей части блока
        unsigned char next_block_value = 0;
        // проверяем дошли ли мы до конца массива
        if (count < block->height) {
            // если не дошли, то инициируем значение следующей части блока
            next_block_value = block->view[count] << block->X;
        }

        /* значение части поля, которая находится ниже текущей части блока для
        ислючения влияния на результат следующих частей блока, вычитаем значение
        следующей части блока, если оно существует. Для точного вычисления значения
        той части поля, которая находится под текущей частью блока побитово умножим (AND) значение части поля на значение текушего блока */
        unsigned char field_value = (field[i] - next_block_value) & block_value;

        // складываем по модулю 2 (XOR) текущее значение блока и значение части поля
        // если результат больше или равен текущему значение блока, то инкрементируем
        // переменную result
        if ((block_value ^ field_value) < block_value) break;
        result++;
    }

    return result;
}
