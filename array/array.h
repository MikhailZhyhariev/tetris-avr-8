#ifndef ARRAY_H
#define ARRAY_H

// На основе массива array, создает двумерный массив, разбивая на отдельные биты каждый член массива array.
unsigned char *Array_BitMap(unsigned char *array, unsigned char rows, unsigned char cols);

// Транспонирует матрицу по главной диагонали
unsigned char *Array_TransposeMainDiagonal(unsigned char *array, unsigned char rows, unsigned char cols);

// Транспонирует матрицу по побочной диагонали
unsigned char *Array_TransposeCollateralDiagonal(unsigned char *array, unsigned char rows, unsigned char cols);

// Поворачивает матрицу на 90 градуосв вправо
unsigned char *Array_RotateRight(unsigned char *array, unsigned char rows, unsigned char cols);

// Из двумерной битовой матрицы array построчно собирает массив чисел
unsigned char *Array_GetHex(unsigned char *array, unsigned char rows, unsigned char cols);

/* Преобразует массив в битовую матрицу, производит определенное действие над битовой матрицей, после
чего снова собирает матрицу в одномерный массив.
Значения аргумента act:
"m" - находит транспонированную по главной диагонали матрицу;
"c" - находит транспонированную по побочной диагонали матрицу;
"r" - поворачивает матрицу вправо на 90 градусов. */
unsigned char *Array_Transform(unsigned char *array, unsigned char *rows, unsigned char *cols, char act);

#endif
