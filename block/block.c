#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "../main.h"
#include "../max7219/max7219.h"
#include "../array/array.h"
#include "block.h"

// массив, в котором содержится информация о блоках
unsigned char block_init_info[7][4] = {
    // widht, height, view
    {4, 1, 0x0F, 0x00},   // I
    {3, 2, 0x01, 0x07},   // J
    {3, 2, 0x04, 0x07},   // L
    {2, 2, 0x03, 0x03},   // O
    {3, 2, 0x06, 0x03},   // S
    {3, 2, 0x02, 0x07},   // T
    {3, 2, 0x03, 0x06},   // Z
};

// Рандомно формирует блок из массива block_init_info
blocks *Block_Init() {
    // рандомно выбираем номер блока
    unsigned char block_number = 0 + rand() % 7;

    // динамически выделяем память под блок
    blocks *block = (blocks*)malloc(sizeof(blocks));

    // задаем ширину, высоту и view блока из массива block_init_info
    block->width = block_init_info[block_number][0];
    block->height = block_init_info[block_number][1];
    block->view = (unsigned char *)malloc(sizeof(unsigned char) * block->height);
    block->view = (unsigned char *)Array_Create(1, block->height);
    for (unsigned char i = 0; i < block->height; i++) {
        block->view[i] = block_init_info[block_number][i + 2];
    }

    // рандомно выбираем X координату
    block->X = 0 + rand() % (FIELD_WIDTH - block->width);
    // Y = 1, из-за того, что адрес первого знака (Digit 0) = 0x01
    block->Y = 1;

    return block;
}

// Добавляет блок на поле сложением (побитовым) с соответствующим значением в массиве field
void Block_Add(blocks *block, unsigned char *field) {
    unsigned char count = 0;
    for (unsigned char i = block->Y; i < block->Y + block->height; i++) {
        field[i - 1] |= block->view[count++] << block->X;
    }
}

// Очищает то место, где находится блок
void Block_Clear(blocks *block, unsigned char *field) {
    unsigned char block_end = block->Y + block->height; // конец блока
    unsigned char count = 0; // индекс массива block.view
    for (unsigned char i = block->Y; i < block_end; i++) {
        // стирание происходит путем сложения по модулю 2 (XOR) части поля и части блока,
        // которая расположена на этой части поля
        field[i - 1] ^= block->view[count++] << block->X;
    }
}

// Двигает блок по полю
blocks *Block_Move(blocks *block, unsigned char *field) {
    unsigned char block_end = block->Y + block->height; // конец блока

    // если блок не дошел до конца поля и значение, которое вернула функция
    // проверки коллизии равно высоте блока, то он смещается по оси Y
    // иначе инициализируем следующий блок
    if ((block_end <= FIELD_HEIGHT) & (Block_Collision(block, field) == block->height)) {

        // очищаем предыдущее расположение блока
        Block_Clear(block, field);
        block->Y++; // смещаем блок по оси Y
    } else {

        if (field[0] > 0x00) memset(field, 0x00, FIELD_HEIGHT);

        // освобождаем память от массива view и блока
        free(block->view);
        free(block);
        block = Block_Init(); // выбираем следующий блок
    }

    // отрисовываем смещенный блок и перерисовываем поле
    Block_Add(block, field);
    MAX_WriteAllDigits(field);

    return block;
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
    поля находится часть блока, то сначала из вычитаем эту часть блока из поля.
    Получаем нужный участок поля (который соответствует расположению блока по оси X)
    путем побитового умножения элемента массива block.view с нужным элементом поля
    после этого складываем по модулю 2 (XOR) полученный участок поля и
    элемент массива block.view. Если результат XOR больше или равен значению соотв.
    элемента block.view, то инкрементируем переменную result (потому что при сдвиге
    этой части блока не возникнет коллизии). */
    for (unsigned char i = block->Y; i < block_end; i++) {

        // значение текущей части блока
        unsigned char block_value = block->view[count++] << block->X;

        // значение следующей части блока
        unsigned char next_block_value = 0;
        // проверяем дошли ли мы до конца массива
        if (count < block->height) {
            // если не дошли, то инициализируем значение следующей части блока
            next_block_value = block->view[count] << block->X;
        }

        /* значение части поля, которая находится ниже текущей части блока для
        ислючения влияния на результат следующих частей блока, вычитаем значение
        следующей части блока, если оно существует. Для точного вычисления значения
        той части поля, которая находится под текущей частью блока побитово умножим
        (AND) значение части поля на значение текушего блока */
        unsigned char field_value = (field[i] - next_block_value) & block_value;

        // складываем по модулю 2 (XOR) текущее значение блока и значение части поля
        // если результат больше или равен текущему значение блока, то инкрементируем
        // переменную result
        if ((block_value ^ field_value) < block_value) break;
        result++;
    }

    return result;
}

// Сдвигает блок влево или вправо по нажатию кнопки
void Block_MoveLeftRight(blocks *block, unsigned char *field) {
    unsigned char block_end = block->X + block->width;

    // если кнопка PD0 нажата и сдвиг блока по оси X больше 0, то сдвигаем блок влево
    if (((PIND&(1 << PD0)) == 0) & (block->X > 0))  {
        // стираем блок перед сдвигом
        Block_Clear(block, field);
        // задержка для того, чтобы одно нажатие кнопки соответствовало сдвигу блока на 1
        _delay_ms(250);
        // сдвигаем блок
        block->X--;
    }
    // если кнопка PD1 нажата и конец блока не выходит за поле, то сдвигаем блок вправо
    else if (((PIND&(1 << PD1)) == 0) & (block_end < FIELD_HEIGHT)) {
        // стираем блок перед сдвигом
        Block_Clear(block, field);
        // задержка для того, чтобы одно нажатие кнопки соответствовало сдвигу блока на 1
        _delay_ms(250);
        // сдвигаем блок
        block->X++;
    }

    // добавляем сдвинутый блок на поле и перерисовываем поле
    Block_Add(block, field);
    MAX_WriteAllDigits(field);
}

// Поворачивает блок по часовой стрелке
blocks *Block_Transform(blocks* block, unsigned char *field) {
    // стираем старый блок
    Block_Clear(block, field);

    // создаем двумерный массив, который содержит битовое представление массива block.view
    unsigned char *bit_view = (unsigned char *)malloc(block->width * block->height * sizeof(unsigned char));
    bit_view = Array_BitMap(block->view, block->height, block->width);

    // создаем двумерный массив для повернутого блока
    unsigned char **rotated_view = Array_Create(block->width, block->height);
    rotated_view = Array_RotateRight(bit_view, block->height, block->width);

    // меняем местами ширину и высоту блока
    Swap(&block->width, &block->height);

    // освобождаем память из-под массива block.view
    free(block->view);
    // выделяем память под повернутый блок
    block->view = (unsigned char *)malloc(sizeof(unsigned char) * block->height);
    block->view = Array_GetHex(rotated_view, block->height, block->width);

    // // освобождаем память из-под массива view_bit
    free(bit_view);
    // // освобождаем память из-под массива rotated_view
    Array_Free(rotated_view);

    // добавляем повернутый блок и перерисосываем поле
    Block_Add(block, field);
    MAX_WriteAllDigits(field);

    return block;
}
