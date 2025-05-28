#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    int size;
    int** data;
} Matrix;

// Функции для работы с матрицами
Matrix create_matrix(int size);
void free_matrix(Matrix matrix);
void print_matrix(Matrix matrix);
Matrix multiply_matrices(Matrix a, Matrix b);

// Параллельное перемножение
typedef struct {
    int thread_id;
    int num_threads;
    Matrix* a;
    Matrix* b;
    Matrix* result;
} ThreadData;

Matrix parallel_matrix_multiply(Matrix a, Matrix b, int num_threads);
void measure_performance(int max_threads, int max_size);

#endif