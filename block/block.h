#ifndef BLOCK_H
#define BLOCK_H

/* Структура, описывающая блок для игры в тетрис.
widht - ширина блока
height - высота блока
X - смещение по оси X
Y - смещение по оси Y
view - массив, члены которого описывают представление блока построчно. */
typedef struct {
    unsigned char width, height;
    unsigned char X, Y;
    unsigned char *view;
} blocks;

// Рандомно формирует блок из массива block_init_info
blocks *Block_Init();

// Добавляет блок на поле сложением (побитовым) с соответствующим значением в массиве field
void Block_Add(blocks *block, unsigned char *field);

// Очищает то место, где находится блок
void Block_Clear(blocks *block, unsigned char *field);

// Двигает блок по полю
blocks *Block_Move(blocks *block, unsigned char *field);

// Проверяет коллизию блока и поля. Возвращает результат поэлементного
// сложения по модулю 2 (XOR) массива block.view и элемента поля, который находится
// ниже соотв. элемента массива block.view
unsigned char Block_Collision(blocks *block, unsigned char *field);

// Сдвигает блок влево или вправо по нажатию кнопки
void Block_MoveLeftRight(blocks *block, unsigned char *field);


blocks *Block_Transform(blocks* block, unsigned char *field);


#endif
