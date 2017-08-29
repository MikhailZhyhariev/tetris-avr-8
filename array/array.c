#include <stdlib.h>
#include "array.h"
#include "../main.h"

// Создает двумерный битовый массив размера (rows х cols)
void **Array_Create(unsigned char rows, unsigned char cols) {
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
unsigned char **Array_BitMap(unsigned char *array, unsigned char rows, unsigned char cols) {
    // создаем двумерный массив размером (rows х cols)
    unsigned char **bit_map = (unsigned char **)Array_Create(rows, cols);

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
unsigned char **Array_TransposeMainDiagonal(unsigned char **array, unsigned char rows, unsigned char cols) {
    // создаем двумерный массив размером (rows х cols)
    unsigned char **transpose_matrix = (unsigned char **)Array_Create(rows, cols);

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // заполняем транспонированную матрицу
            transpose_matrix[i][j] = array[j][i];
        }
    }

    return transpose_matrix;
}

// Транспонирует матрицу по побочной диагонали
unsigned char **Array_TransposeCollateralDiagonal(unsigned char **array, unsigned char rows, unsigned char cols) {
    // создаем двумерный массив размером (rows х cols)
    unsigned char **transpose_matrix = (unsigned char **)Array_Create(rows, cols);

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // заполняем транспонированную матрицу
            transpose_matrix[i][j] = array[cols - 1 - j][rows - 1 - i];
        }
    }

    return transpose_matrix;
}

// Из двумерной битовой матрицы array построчно собирает массив чисел
unsigned char *Array_GetHex(unsigned char **array, unsigned char rows, unsigned char cols) {
    // создаем массив размером (rows)
    unsigned char *transpose_field = (unsigned char *)Array_Create(rows, 1);
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
