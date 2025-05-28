#include "matrix_operations.h"
#include <stdlib.h>

Matrix create_matrix(int rows, int cols) {
    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        m.data[i] = (double*)malloc(cols * sizeof(double));
    }
    return m;
}

void free_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->data[i]);
    }
    free(m->data);
    m->rows = 0;
    m->cols = 0;
}

Matrix add_matrices(const Matrix *a, const Matrix *b) {
    Matrix result = create_matrix(a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result.data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

Matrix multiply_matrices(const Matrix *a, const Matrix *b) {
    Matrix result = create_matrix(a->rows, b->cols);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                result.data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return result;
}

Matrix transpose_matrix(const Matrix *m) {
    Matrix result = create_matrix(m->cols, m->rows);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result.data[j][i] = m->data[i][j];
        }
    }
    return result;
}

double matrix_determinant(const Matrix *m) {
    if (m->rows != m->cols) return 0;
    // Упрощённая реализация для матриц 2x2
    if (m->rows == 2) {
        return m->data[0][0] * m->data[1][1] - m->data[0][1] * m->data[1][0];
    }
    // Для больших матриц нужно реализовать разложение
    return 0;
}