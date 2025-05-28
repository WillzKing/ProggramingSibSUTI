#include "matrix_io.h"
#include <stdio.h>

void print_matrix(const Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%6.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}

Matrix read_matrix_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        Matrix empty = {0, 0, NULL};
        return empty;
    }
    
    int rows, cols;
    fscanf(file, "%d %d", &rows, &cols);
    Matrix m = create_matrix(rows, cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%lf", &m.data[i][j]);
        }
    }
    
    fclose(file);
    return m;
}

void write_matrix_to_file(const char *filename, const Matrix *m) {
    FILE *file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "%d %d\n", m->rows, m->cols);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            fprintf(file, "%f ", m->data[i][j]);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
}