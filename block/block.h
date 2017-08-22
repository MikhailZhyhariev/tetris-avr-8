#ifndef BLOCK_H
#define BLOCK_H

#include "../main.h"

void Block_Add(blocks *block, unsigned char *field);

void Block_Move(blocks *block, unsigned char *field);

unsigned char Block_Collision(blocks *block, unsigned char *field);


#endif
