#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

// Структура для хранения матрицы
typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

// Прототипы функций для операций с матрицами
Matrix create_matrix(int rows, int cols);
void free_matrix(Matrix *m);
Matrix add_matrices(const Matrix *a, const Matrix *b);
Matrix multiply_matrices(const Matrix *a, const Matrix *b);
Matrix transpose_matrix(const Matrix *m);
double matrix_determinant(const Matrix *m);

#endif