#ifndef FIELD_H
#define FIELD_H

// Создает двумерный битовый массив размера (rows х cols)
void **FIELD_CreateArray(unsigned char rows, unsigned char cols);

// На основе массива array, создает двумерный массив, разбивая на отдельные биты каждый член массива array.
unsigned char **FIELD_BitMap(unsigned char *array, unsigned char rows, unsigned char cols);

// Транспонирует матрицу по главной диагонали
unsigned char **FIELD_TransposeMainDiagonal(unsigned char **array, unsigned char rows, unsigned char cols);

// Транспонирует матрицу по побочной диагонали
unsigned char **FIELD_TransposeCollateralDiagonal(unsigned char **array, unsigned char rows, unsigned char cols);

// Из двумерной битовой матрицы array построчно собирает массив чисел
unsigned char *FIELD_GetHexArray(unsigned char **array, unsigned char rows, unsigned char cols);


#endif
