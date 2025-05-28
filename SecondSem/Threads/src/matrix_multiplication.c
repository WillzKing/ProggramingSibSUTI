#include "../include/matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Matrix create_matrix(int size) {
    Matrix mat;
    mat.size = size;
    mat.data = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        mat.data[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            mat.data[i][j] = 1;
        }
    }
    return mat;
}

void free_matrix(Matrix matrix) {
    for (int i = 0; i < matrix.size; i++) {
        free(matrix.data[i]);
    }
    free(matrix.data);
}

void print_matrix(Matrix matrix) {
    if (matrix.size > 5) {
        printf("Matrica slishkom bolshaya dlya pechati (%dx%d)\n", matrix.size, matrix.size);
        return;
    }
    
    for (int i = 0; i < matrix.size; i++) {
        for (int j = 0; j < matrix.size; j++) {
            printf("%d ", matrix.data[i][j]);
        }
        printf("\n");
    }
}

Matrix multiply_matrices(Matrix a, Matrix b) {
    Matrix result = create_matrix(a.size);
    
    for (int i = 0; i < a.size; i++) {
        for (int j = 0; j < a.size; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < a.size; k++) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    
    return result;
}

void* multiply_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int rows_per_thread = data->result->size / data->num_threads;
    int start_row = data->thread_id * rows_per_thread;
    int end_row = (data->thread_id + 1) * rows_per_thread;
    
    if (data->thread_id == data->num_threads - 1) {
        end_row = data->result->size;
    }
    
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < data->result->size; j++) {
            data->result->data[i][j] = 0;
            for (int k = 0; k < data->result->size; k++) {
                data->result->data[i][j] += data->a->data[i][k] * data->b->data[k][j];
            }
        }
    }
    
    return NULL;
}

Matrix parallel_matrix_multiply(Matrix a, Matrix b, int num_threads) {
    Matrix result = create_matrix(a.size);
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = num_threads;
        thread_data[i].a = &a;
        thread_data[i].b = &b;
        thread_data[i].result = &result;
        pthread_create(&threads[i], NULL, multiply_thread, &thread_data[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return result;
}

void measure_performance(int max_threads, int max_size) {
    printf("\n=== Zamer proizvoditelnosti ===\n");
    printf("Razmer matritsi\tPotoki\tVremya (ms)\n");
    
    for (int size = 100; size <= max_size; size += 100) {
        Matrix a = create_matrix(size);
        Matrix b = create_matrix(size);
        
        for (int threads = 1; threads <= max_threads; threads *= 2) {
            clock_t start = clock();
            Matrix c = parallel_matrix_multiply(a, b, threads);
            clock_t end = clock();
            
            double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
            printf("%dx%d\t\t%d\t%.2f\n", size, size, threads, time_spent);
            
            free_matrix(c);
        }
        
        free_matrix(a);
        free_matrix(b);
    }
}