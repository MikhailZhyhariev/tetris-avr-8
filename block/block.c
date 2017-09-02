#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "../main.h"
#include "../max7219/max7219.h"
#include "../array/array.h"
#include "block.h"

// массив, в котором содержится информация о блоках
unsigned char block_init_info[7][4] = {
    // width, height, view
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
    unsigned char block_number = 2;//0 + rand() % 7;

    // динамически выделяем память под блок
    blocks *block = (blocks*)malloc(sizeof(blocks));

    // задаем ширину, высоту и view блока из массива block_init_info
    block->width = block_init_info[block_number][0];
    block->height = block_init_info[block_number][1];
    block->view = (unsigned char *)malloc(sizeof(unsigned char) * block->height);
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

/* Копирует блок с возможностью преобразования (которая задается аргументом act) и возвращает копию
значения аргумента act:
"n" - возвращает блок без преобразований
"m" - находит транспонированную по главной диагонали матрицу;
"c" - находит транспонированную по побочной диагонали матрицу;
"r" - поворачивает блок вправо на 90 градусов. */
blocks *Block_Copy(blocks *block, char act) {
    // выделяем память под копию блока
    blocks *c_block = (blocks *)malloc(sizeof(blocks));
    // задаем ширину и высоту нового блока
    c_block->width = block->width;
    c_block->height = block->height;
    // выделяем память под массив block.view
    c_block->view = (unsigned char *)malloc(block->height * sizeof(unsigned char));
    // задаем положение на поле
    c_block->X = block->X;
    c_block->Y = block->Y;

    // транспонируем по побочной диагонали
    if (act == 'c') c_block->view = Array_Transform(block->view, &c_block->height, &c_block->width, 'c');
    // транспонируем по главной диагонали
    else if (act == 'm' ) c_block->view = Array_Transform(block->view, &c_block->height, &c_block->width, 'm');
    // поворачиваем блок на 90 градусов вправо
    else if (act == 'r') c_block->view = Array_Transform(block->view, &c_block->height, &c_block->width, 'r');
    // копируем массив block.view в новый блок
    else {
        for (unsigned char i = 0; i < block->height; i++) {
            c_block->view[i] = block->view[i];
        }
    }

    return c_block;
}

// Двигает блок по полю
blocks *Block_MoveDown(blocks *block, unsigned char *field) {
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

// Сдвигает блок влево
void Block_MoveLeft(blocks *block, unsigned char *field) {
    // определяем произойдет ли коллизия при сдвиге
    unsigned char collision = Block_SideCollision(block, field, 'l');

    // если блок не выходит за левую границу поля и если не произойдет коллизия при сдвиге,
    // то сдвигаем на 1 влево
    if ((block->X > 0) & collision) {
        // запрещаем прерывания
        cli();
        // стираем блок перед сдвигом
        Block_Clear(block, field);
        // сдвигаем блок
        block->X--;
        // добавляем сдвинутый блок на поле и перерисовываем поле
        Block_Add(block, field);
        MAX_WriteAllDigits(field);
        // разрешаем прерывания
        sei();
    }
}

// Сдвигаем блок на 1 вправо
void Block_MoveRight(blocks *block, unsigned char *field) {
    // определяем произойдет ли коллизия при сдвиге
    unsigned char collision = Block_SideCollision(block, field, 'r');

    // если блок не выходит за правую границу поля и если коллизия при сдвиге не произойдет,
    // то сдвигаем на 1 вправо
    if ((block->X + block->width < FIELD_WIDTH) & collision) {
        // запрещаем прерывания
        cli();
        // стираем блок перед сдвигом
        Block_Clear(block, field);
        // сдвигаем блок
        block->X++;
        // добавляем сдвинутый блок на поле и перерисовываем поле
        Block_Add(block, field);
        MAX_WriteAllDigits(field);
        // разрешаем прерывания
        sei();
    }
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

unsigned char Block_SideCollision(blocks *block, unsigned char *field, unsigned char side) {
    // определяем размеры поля
    unsigned char field_height = FIELD_HEIGHT;
    unsigned char field_width = FIELD_WIDTH;

    // исходя из направления сдвига определяем трансформацию матрицу
    char transform;
    if (side == 'r') transform = 'm';
    else transform = 'c';

    // удаляем блок с поля, чтобы транспонировать поле без блока
    Block_Clear(block, field);

    // выделяем память под поле, для определения коллизии слева
    unsigned char *collision_field = (unsigned char *)malloc(field_width * sizeof(unsigned char));
    // транспонируем поле по побочной диагонали
    collision_field = Array_Transform(field, &field_height, &field_width, transform);

    // добавляем блок назад
    Block_Add(block, field);

    // копируем блок и транспонируем его по побочной диагонали
    blocks *collision_block = (blocks *)malloc(sizeof(blocks));
    collision_block = Block_Copy(block, transform);

    // переопределяем смещение в соотв со сдвинутым полем и направлением сдвига блока
    if (transform == 'c') {
        collision_block->Y = field_width - (block->X + block->width) + 1;
        collision_block->X = field_height - (block->Y + block->height) + 1;
    } else {
        collision_block->Y = block->X + 1;
        collision_block->X = block->Y - 1;
    }

    // добавляем скопированный блок на поле collision_field
    Block_Add(collision_block, collision_field);

    unsigned char result = 0;
    if (Block_Collision(collision_block, collision_field) == collision_block->height) result = 1;

    // освобождаем память из-под массивов
    free(collision_block);
    free(collision_field);

    return result;
}

// Поворачивает блок по часовой стрелке
void Block_Rotate(blocks* block, unsigned char *field) {
    // стираем старый блок
    Block_Clear(block, field);

    // переопределяем массив block.view
    block->view = Array_Transform(block->view, &block->height, &block->width, 'r');

    // добавляем повернутый блок и перерисосываем поле
    Block_Add(block, field);
    MAX_WriteAllDigits(field);
}
