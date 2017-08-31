#ifndef ARRAY_H
#define ARRAY_H

// Создает двумерный битовый массив размера (rows х cols)
unsigned char **Array_Create(unsigned char rows, unsigned char cols);

// Освобождаем память из-под двумерного массива
void Array_Free(unsigned char** array);

// На основе массива array, создает двумерный массив, разбивая на отдельные биты каждый член массива array.
unsigned char *Array_BitMap(unsigned char *array, unsigned char rows, unsigned char cols);

// Транспонирует матрицу по главной диагонали
unsigned char **Array_TransposeMainDiagonal(unsigned char *array, unsigned char rows, unsigned char cols);

// Транспонирует матрицу по побочной диагонали
unsigned char **Array_TransposeCollateralDiagonal(unsigned char *array, unsigned char rows, unsigned char cols);

// Поворачивает матрицу на 90 градуосв вправо
unsigned char **Array_RotateRight(unsigned char *array, unsigned char rows, unsigned char cols);

// Из двумерной битовой матрицы array построчно собирает массив чисел
unsigned char *Array_GetHex(unsigned char **array, unsigned char rows, unsigned char cols);


#endif
