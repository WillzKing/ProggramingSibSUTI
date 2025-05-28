#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

typedef struct {
    int** data;
    int size;
} Matrix;

typedef struct {
    int thread_id;
    int num_threads;
    Matrix* a;
    Matrix* b;
    Matrix* result;
} ThreadData;

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

void free_matrix(Matrix mat) {
    for (int i = 0; i < mat.size; i++) {
        free(mat.data[i]);
    }
    free(mat.data);
}

void print_matrix(Matrix mat) {
    for (int i = 0; i < mat.size; i++) {
        for (int j = 0; j < mat.size; j++) {
            printf("%d ", mat.data[i][j]);
        }
        printf("\n");
    }
}

void* multiply_part(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int rows_per_thread = data->a->size / data->num_threads;
    int start_row = data->thread_id * rows_per_thread;
    int end_row = (data->thread_id + 1) * rows_per_thread;
    
    if (data->thread_id == data->num_threads - 1) {
        end_row = data->a->size;
    }
    
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < data->b->size; j++) {
            data->result->data[i][j] = 0;
            for (int k = 0; k < data->a->size; k++) {
                data->result->data[i][j] += data->a->data[i][k] * data->b->data[k][j];
            }
        }
    }
    return NULL;
}

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void benchmark(int max_threads, int max_size) {
    FILE *file = fopen("benchmark_results.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    // Заголовок файла
    fprintf(file, "Threads\tSize\tTime(ms)\n");
    printf("Threads\tSize\tTime(ms)\n");
    
    for (int size = 100; size <= max_size; size += 200) {
        for (int threads = 1; threads <= max_threads; threads *= 2) {
            Matrix a = create_matrix(size);
            Matrix b = create_matrix(size);
            Matrix result = create_matrix(size);
            
            pthread_t* thread_handles = (pthread_t*)malloc(threads * sizeof(pthread_t));
            ThreadData* thread_data = (ThreadData*)malloc(threads * sizeof(ThreadData));
            
            double start = get_time();
            
            for (int i = 0; i < threads; i++) {
                thread_data[i].thread_id = i;
                thread_data[i].num_threads = threads;
                thread_data[i].a = &a;
                thread_data[i].b = &b;
                thread_data[i].result = &result;
                pthread_create(&thread_handles[i], NULL, multiply_part, &thread_data[i]);
            }
            
            for (int i = 0; i < threads; i++) {
                pthread_join(thread_handles[i], NULL);
            }
            
            double end = get_time();
            double elapsed = (end - start) * 1000;
            
            // Запись в файл и вывод на экран
            fprintf(file, "%d\t%d\t%.2f\n", threads, size, elapsed);
            printf("%d\t%d\t%.2f\n", threads, size, elapsed);
            
            if (size <= 4) {
                printf("\nMatrix A:\n");
                print_matrix(a);
                printf("\nMatrix B:\n");
                print_matrix(b);
                printf("\nResult Matrix:\n");
                print_matrix(result);
            }
            
            free(thread_handles);
            free(thread_data);
            free_matrix(a);
            free_matrix(b);
            free_matrix(result);
        }
    }
    
    fclose(file);
    printf("\nBenchmark results saved to 'benchmark_results.txt'\n");
}

int main(int argc, char* argv[]) {
    if (argc == 2 && strcmp(argv[1], "benchmark") == 0) {
        benchmark(128, 2500);
        return 0;
    }
    
    if (argc != 3) {
        printf("Usage:\n");
        printf("  For single test: %s <matrix_size> <thread_count>\n", argv[0]);
        printf("  For benchmark:   %s benchmark\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    int threads = atoi(argv[2]);

    if (size <= 0 || threads <= 0) {
        printf("Parameters must be positive integers\n");
        return 1;
    }

    Matrix a = create_matrix(size);
    Matrix b = create_matrix(size);
    Matrix result = create_matrix(size);
    
    pthread_t* thread_handles = (pthread_t*)malloc(threads * sizeof(pthread_t));
    ThreadData* thread_data = (ThreadData*)malloc(threads * sizeof(ThreadData));
    
    double start = get_time();
    
    for (int i = 0; i < threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = threads;
        thread_data[i].a = &a;
        thread_data[i].b = &b;
        thread_data[i].result = &result;
        pthread_create(&thread_handles[i], NULL, multiply_part, &thread_data[i]);
    }
    
    for (int i = 0; i < threads; i++) {
        pthread_join(thread_handles[i], NULL);
    }
    
    double end = get_time();
    printf("Execution time: %.2f ms\n", (end - start) * 1000);
    
    if (size <= 4) {
        printf("\nMatrix A:\n");
        print_matrix(a);
        printf("\nMatrix B:\n");
        print_matrix(b);
        printf("\nResult Matrix:\n");
        print_matrix(result);
    }
    
    free(thread_handles);
    free(thread_data);
    free_matrix(a);
    free_matrix(b);
    free_matrix(result);
    
    return 0;
}
