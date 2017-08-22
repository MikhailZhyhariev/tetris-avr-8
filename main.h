#ifndef MAIN_H
#define MAIN_H

#define FIELD_HEIGHT 8
#define FIELD_WIDTH 8

/* Структура, описывающая блок для игры в тетрис.
widht - ширина блока
height - высота блока
X - смещение по оси X
Y - смещение по оси Y
view - массив, члены которого описывают представление блока построчно. */
typedef struct {
    unsigned char width, height;
    unsigned char X, Y;
    unsigned char view[];
} blocks;



#endif
