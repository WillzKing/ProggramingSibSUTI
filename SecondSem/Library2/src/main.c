#include <stdio.h>
#include "matrix_operations.h"
#include "matrix_io.h"

int main() {
    // Создаём две матрицы
    Matrix a = create_matrix(2, 2);
    Matrix b = create_matrix(2, 2);
    
    // Заполняем матрицы
    a.data[0][0] = 1; a.data[0][1] = 2;
    a.data[1][0] = 3; a.data[1][1] = 4;
    
    b.data[0][0] = 5; b.data[0][1] = 6;
    b.data[1][0] = 7; b.data[1][1] = 8;
    
    printf("Matrix A:\n");
    print_matrix(&a);
    
    printf("\nMatrix B:\n");
    print_matrix(&b);
    
    // Сложение матриц
    Matrix sum = add_matrices(&a, &b);
    printf("\nSum of A and B:\n");
    print_matrix(&sum);
    
    // Умножение матриц
    Matrix product = multiply_matrices(&a, &b);
    printf("\nProduct of A and B:\n");
    print_matrix(&product);
    
    // Транспонирование
    Matrix transposed = transpose_matrix(&a);
    printf("\nTransposed A:\n");
    print_matrix(&transposed);
    
    // Определитель
    printf("\nDeterminant of A: %.2f\n", matrix_determinant(&a));
    
    // Освобождаем память
    free_matrix(&a);
    free_matrix(&b);
    free_matrix(&sum);
    free_matrix(&product);
    free_matrix(&transposed);
    
    return 0;
}