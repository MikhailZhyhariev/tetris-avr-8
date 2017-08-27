#include <stdlib.h>
#include "field.h"
#include "../main.h"

// Создает двумерный битовый массив размера (rows х cols)
void **FIELD_CreateArray(unsigned char rows, unsigned char cols) {
    // выделяем память под строки матрицы
    void **array = (void **)malloc(sizeof(void *) * rows);
    for (unsigned char i = 0; i < rows; i++) {
        // заполняем строки матрицы
        array[i] = (void *)malloc(sizeof(void) * cols);
    }

    // возращает безымянный указатель, поэтому нужен явный каст в нужный тип при инициализации массива
    return array;
}

// На основе массива array, создает двумерный массив, разбивая на отдельные биты каждый член массива array.
unsigned char **FIELD_BitMap(unsigned char *array, unsigned char rows, unsigned char cols) {
    // создаем двумерный массив размером (rows х cols)
    unsigned char **bit_map = (unsigned char **)FIELD_CreateArray(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // переменная temp выделяет j-ый бит из члена массива array
            unsigned char temp = array[i] & (1 << j);
            // сдвигаем на j разрядов вправо, чтобы бит temp стал 0-вым битом и записывает в
            // результирующую матрицу
            bit_map[i][j] = temp >> j;
        }
    }

    return bit_map;
}

// Транспонирует матрицу по главной диагонали
unsigned char **FIELD_TransposeMainDiagonal(unsigned char **array, unsigned char rows, unsigned char cols) {
    // создаем двумерный массив размером (rows х cols)
    unsigned char **transpose_matrix = (unsigned char **)FIELD_CreateArray(rows, cols);

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            transpose_matrix[i][j] = array[j][i];
        }
    }

    return transpose_matrix;
}

// Транспонирует матрицу по побочной диагонали
unsigned char **FIELD_TransposeCollateralDiagonal(unsigned char **array, unsigned char rows, unsigned char cols) {
    // создаем двумерный массив размером (rows х cols)
    unsigned char **transpose_matrix = (unsigned char **)FIELD_CreateArray(rows, cols);

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            transpose_matrix[i][j] = array[rows - 1 - j][cols - 1 - i];
        }
    }

    return transpose_matrix;
}

// Из двумерной битовой матрицы array построчно собирает массив чисел
unsigned char *FIELD_GetHexArray(unsigned char **array, unsigned char rows, unsigned char cols) {
    // создаем массив размером (rows)
    unsigned char *transpose_field = (unsigned char *)FIELD_CreateArray(rows, 1);
    free(transpose_field); // очищаем память

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // собираем строки матрицы array в одно шестнадцатеричное число и записываем в
            // результирующий массив
            transpose_field[i] += array[i][j] << j;
        }
    }

    return transpose_field;
}
