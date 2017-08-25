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

blocks *Block_Init();

void Block_Add(blocks *block, unsigned char *field);

blocks *Block_Move(blocks *block, unsigned char *field);

unsigned char Block_Collision(blocks *block, unsigned char *field);

void Block_Clear(blocks *block, unsigned char *field);


#endif
