#include "../main.h"
#include "../max7219/max7219.h"

// Добавляем блок на поле сложением (побитовым) с соответствующим значением в массиве field
void Block_Add(blocks *block, unsigned char *field) {
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
    Block_Add(block, field);
    MAX_WriteAllDigits(field);
}
