#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "../main.h"

// Создает двумерный битовый массив размера (rows х cols)
unsigned char **Array_Create(unsigned char rows, unsigned char cols) {
    // выделяем память под строки матрицы
    unsigned char **array = (unsigned char **)malloc(sizeof(unsigned char *) * rows);
    // выделяем память под столбцы матрицы
    unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * rows * cols);
    for (unsigned char i = 0; i < rows; i++) {
        array[i] = data + i * cols;
    }

    return array;
}

// Освобождаем память из-под двумерного массива
void Array_Free(unsigned char** array) {
    free(array[0]);
    free(array);
}

// На основе массива array, создает двумерный массив, разбивая на отдельные биты каждый член массива array.
unsigned char *Array_BitMap(unsigned char *array, unsigned char rows, unsigned char cols) {
    // создаем двумерный массив размером (rows х cols)
    unsigned char *bit_map = (unsigned char *)malloc(rows * cols * sizeof(unsigned char));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // переменная temp выделяет j-ый бит из члена массива array
            unsigned char temp = array[i] & (1 << j);
            // сдвигаем на j разрядов вправо, чтобы бит temp стал 0-вым битом и записывает в
            // результирующую матрицу
            *(bit_map + i * cols + j) = temp >> j;
        }
    }
    return bit_map;
}

// Транспонирует матрицу по главной диагонали
unsigned char **Array_TransposeMainDiagonal(unsigned char *array, unsigned char rows, unsigned char cols) {
    // создаем двумерный массив размером (rows х cols)
    unsigned char **transpose_matrix = Array_Create(rows, cols);

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // заполняем транспонированную матрицу
            transpose_matrix[i][j] = *(array + j * rows + i);
        }
    }

    return transpose_matrix;
}

// Транспонирует матрицу по побочной диагонали
unsigned char **Array_TransposeCollateralDiagonal(unsigned char *array, unsigned char rows, unsigned char cols) {
    // создаем двумерный массив размером (rows х cols)
    unsigned char **transpose_matrix = Array_Create(rows, cols);

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // заполняем транспонированную матрицу
            transpose_matrix[i][j] = *(array + (cols - 1 - j) * rows + (rows - 1 - i));
        }
    }

    return transpose_matrix;
}

// Поворачивает матрицу на 90 градуосв вправо
unsigned char **Array_RotateRight(unsigned char *array, unsigned char rows, unsigned char cols) {
    // выделяем память под двумерный массив размером (cols x rows)
    unsigned char **rotated_array = Array_Create(cols, rows);

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // заполняем матрицу
            rotated_array[j][rows - i - 1] = *(array + i * cols + j);
        }
    }

    return rotated_array;
}

// Из двумерной битовой матрицы array построчно собирает массив чисел
unsigned char *Array_GetHex(unsigned char **array, unsigned char rows, unsigned char cols) {
    // создаем массив размером (rows)
    unsigned char *transpose_field = (unsigned char *)malloc(rows * sizeof(unsigned char));
    memset(transpose_field, 0, rows); // обнуляем массив

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // собираем строки матрицы array в одно шестнадцатеричное число и записываем в
            // результирующий массив
            transpose_field[i] += array[i][j] << j;
            // *(array + i * cols + j) << j;
        }
    }

    return transpose_field;
}
