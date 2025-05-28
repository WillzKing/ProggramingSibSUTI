#include "../include/threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int id;
    const char** messages;
    int count;
} ThreadParams;

void* thread_function(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&output_mutex);
        printf("Docherniy potok: stroka %d\n", i+1);
        pthread_mutex_unlock(&output_mutex);
    }
    return NULL;
}

void* thread_function_with_param(void* param) {
    ThreadParams* params = (ThreadParams*)param;
    for (int i = 0; i < params->count; i++) {
        pthread_mutex_lock(&output_mutex);
        printf("Potok %d: %s\n", params->id, params->messages[i]);
        pthread_mutex_unlock(&output_mutex);
    }
    return NULL;
}

void cleanup_handler(void* arg) {
    pthread_mutex_lock(&output_mutex);
    printf("Potok zavershaetsya s cleanup: %s\n", (char*)arg);
    pthread_mutex_unlock(&output_mutex);
}

void* thread_function_with_cleanup(void* param) {
    char* message = "Soobshenie pri zavershenii";
    pthread_cleanup_push(cleanup_handler, message);
    
    // Флаг для контроля работы потока
    volatile int keep_running = 1;
    
    while (keep_running) {
        pthread_mutex_lock(&output_mutex);
        printf("Potok rabotaet...\n");
        pthread_mutex_unlock(&output_mutex);
        
        // Добавляем проверку точки отмены
        pthread_testcancel();
        sleep(1);
    }
    
    pthread_cleanup_pop(0);
    return NULL;
}

void* sleepsort_thread(void* arg) {
    int value = *(int*)arg;
    sleep(value);
    printf("%d ", value);
    return NULL;
}

void demonstrate_thread_creation() {
    printf("\n=== Zadanie 1: Sozdanie potoka ===\n");
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    
    for (int i = 0; i < 5; i++) {
        printf("Roditelskiy potok: stroka %d\n", i+1);
    }
    
    pthread_join(thread, NULL);
}

void demonstrate_thread_joining() {
    printf("\n=== Zadanie 2: Ozidanie potoka ===\n");
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    pthread_join(thread, NULL);
    
    for (int i = 0; i < 5; i++) {
        printf("Roditelskiy potok posle join: stroka %d\n", i+1);
    }
}

void demonstrate_thread_parameters() {
    printf("\n=== Zadanie 3: Parametry potoka ===\n");
    const char* messages1[] = {"Soobshenie 1.1", "Soobshenie 1.2", "Soobshenie 1.3"};
    const char* messages2[] = {"Soobshenie 2.1", "Soobshenie 2.2"};
    const char* messages3[] = {"Soobshenie 3.1", "Soobshenie 3.2", "Soobshenie 3.3", "Soobshenie 3.4"};
    const char* messages4[] = {"Soobshenie 4.1"};
    
    ThreadParams params[] = {
        {1, messages1, 3},
        {2, messages2, 2},
        {3, messages3, 4},
        {4, messages4, 1}
    };
    
    pthread_t threads[4];
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_function_with_param, &params[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
}

void demonstrate_thread_cancellation() {
    printf("\n=== Zadanie 4: Zavershenie potoka bez ozidaniya ===\n");
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function_with_cleanup, NULL);
    
    sleep(2); // Даем потоку поработать 2 секунды
    
    // Отправляем запрос на отмену потока
    printf("Otpravlyaem zapros na otmenu potoka...\n");
    pthread_cancel(thread);
    
    // Ожидаем завершения потока
    if (pthread_join(thread, NULL) == 0) {
        printf("Potok uspeshno zavershen\n");
    } else {
        printf("Oshibka pri zavershenii potoka\n");
    }
}

void demonstrate_thread_cleanup() {
    printf("\n=== Zadanie 5: Obrabotka zaversheniya potoka ===\n");
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function_with_cleanup, NULL);
    
    sleep(2);
    pthread_cancel(thread);
    pthread_join(thread, NULL);
}

void demonstrate_sleepsort(int* array, int size) {
    printf("\n=== Zadanie 6: Sleepsort ===\nIshodnyi massiv: ");
    for (int i = 0; i < size; i++) printf("%d ", array[i]);
    printf("\nOtsortirovannyi massiv: ");
    
    pthread_t threads[size];
    for (int i = 0; i < size; i++) {
        pthread_create(&threads[i], NULL, sleepsort_thread, &array[i]);
    }
    
    for (int i = 0; i < size; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("\n");
}