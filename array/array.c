#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "../main.h"

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
unsigned char *Array_TransposeMainDiagonal(unsigned char *array, unsigned char rows, unsigned char cols) {
    // создаем массив размером (rows х cols)
    unsigned char *transpose_matrix = (unsigned char *)malloc(rows * cols * sizeof(unsigned char));

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // заполняем транспонированную матрицу
            *(transpose_matrix + i * cols + j) = *(array + j * rows + i);
        }
    }

    return transpose_matrix;
}

// Транспонирует матрицу по побочной диагонали
unsigned char *Array_TransposeCollateralDiagonal(unsigned char *array, unsigned char rows, unsigned char cols) {
    // создаем массив размером (rows х cols)
    unsigned char *transpose_matrix = (unsigned char *)malloc(rows * cols * sizeof(unsigned char));

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // заполняем транспонированную матрицу
            *(transpose_matrix + i * cols + j) = *(array + (cols - 1 - j) * rows + (rows - 1 - i));
        }
    }

    return transpose_matrix;
}

// Поворачивает матрицу на 90 градусов вправо
unsigned char *Array_RotateRight(unsigned char *array, unsigned char rows, unsigned char cols) {
    // выделяем память под массив размером (cols x rows)
    unsigned char *rotated_array = (unsigned char *)malloc(rows * cols * sizeof(unsigned char));

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // заполняем матрицу
            *(rotated_array + j * rows + (rows - i - 1)) = *(array + i * cols + j);
        }
    }

    return rotated_array;
}

// Из двумерной битовой матрицы array построчно собирает массив чисел
unsigned char *Array_GetHex(unsigned char *array, unsigned char rows, unsigned char cols) {
    // создаем массив размером (rows)
    unsigned char *transpose_field = (unsigned char *)malloc(rows * sizeof(unsigned char));
    memset(transpose_field, 0, rows); // обнуляем массив

    for (unsigned char i = 0; i < rows; i++) {
        for (unsigned char j = 0; j < cols; j++) {
            // собираем строки матрицы array в одно шестнадцатеричное число и записываем в
            // результирующий массив
            transpose_field[i] += *(array + i * cols + j) << j;
        }
    }

    return transpose_field;
}

/* Преобразует массив в битовую матрицу, производит определенное действие над битовой матрицей, после
чего снова собирает матрицу в одномерный массив.
Значения аргумента act:
"m" - находит транспонированную по главной диагонали матрицу;
"c" - находит транспонированную по побочной диагонали матрицу;
"r" - поворачивает матрицу вправо на 90 градусов. */
unsigned char *Array_Transform(unsigned char *array, unsigned char *rows, unsigned char *cols, char act) {
    // создаем массив, который содержит битовое представление массива array
    unsigned char *bit_array = (unsigned char *)malloc((*rows) * (*cols) * sizeof(unsigned char));
    bit_array = Array_BitMap(array, *rows, *cols);

    // создаем массив для преобразованного битового представления
    unsigned char *transformed_array = (unsigned char *)malloc((*rows) * (*cols) * sizeof(unsigned char));
    // транспонируем по главной диагонали
    if (act == 'm') transformed_array = Array_TransposeMainDiagonal(bit_array, *rows, *cols);
    // транспонируем по побочной диагонали
    else if (act == 'c') transformed_array = Array_TransposeCollateralDiagonal(bit_array, *rows, *cols);
    // поворачиваем на 90 градусов вправо
    else if (act == 'r') transformed_array = Array_RotateRight(bit_array, *rows, *cols);

    // меняем местами строки и столбцы
    Swap(rows, cols);

    // выделяем память под массив и собираем матрицу построчно
    unsigned char *new_array = (unsigned char *)malloc((*rows) * sizeof(unsigned char));
    new_array = Array_GetHex(transformed_array, *rows, *cols);

    // освобождаем память из-под массив
    free(bit_array);
    free(transformed_array);

    return new_array;
}
