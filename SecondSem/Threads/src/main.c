#include <stdio.h>
#include "../include/threads.h"
#include "../include/matrix.h"
#include "../include/queue.h"
#include "../include/philosophers.h"
void demonstrate_message_queue(void);

int main() {
    printf("=== Nachalo raboty s potokami (otsenka 3) ===\n");
    
    demonstrate_thread_creation();
    demonstrate_thread_joining();
    demonstrate_thread_parameters();
    demonstrate_thread_cancellation();
    demonstrate_thread_cleanup();
    

    int sleepsort_array[] = {3, 1, 4, 1, 5, 9, 2, 6};
    demonstrate_sleepsort(sleepsort_array, 8);

    printf("\n=== Demonstratsiya peremnozheniya matrits (otsenka 4) ===\n");
    
    Matrix a = create_matrix(3);
    Matrix b = create_matrix(3);

    // Заполняем матрицы тестовыми данными
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a.data[i][j] = i + j;
            b.data[i][j] = i * j;
        }
    }

    Matrix c = parallel_matrix_multiply(a, b, 2); // Умножаем с 2 потоками
    print_matrix(c);

    // Освобождаем память
    free_matrix(a);
    free_matrix(b);
    free_matrix(c);

    // Замеры производительности
    printf("\n=== Zamery proizvoditel'nosti ===\n");
    measure_performance(8, 100); // 8 потоков, 100 итераций

    printf("\n=== Demonstratsiya ocheredi soobshcheniy (otsenka 5) ===\n");
    demonstrate_message_queue();

    printf("\n=== Demonstratsiya zadachi obedayushchikh filosofov ===\n");
    dine_philosophers();

    return 0;
}