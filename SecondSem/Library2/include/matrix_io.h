#ifndef MATRIX_IO_H
#define MATRIX_IO_H

#include "matrix_operations.h"

// Прототипы функций для ввода/вывода матриц
void print_matrix(const Matrix *m);
Matrix read_matrix_from_file(const char *filename);
void write_matrix_to_file(const char *filename, const Matrix *m);

#endif