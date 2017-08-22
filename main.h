#ifndef MAIN_H
#define MAIN_H

#define FIELD_HEIGHT 8
#define FIELD_WIDTH 8

typedef struct {
    unsigned char width, height;
    unsigned char X, Y;
    unsigned char view[];
} blocks;



#endif
