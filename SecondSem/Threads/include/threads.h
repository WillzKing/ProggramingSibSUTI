#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

// Функции для заданий 1-6
void* thread_function(void* arg);
void* thread_function_with_param(void* param);
void* thread_function_with_cleanup(void* param);
void* sleepsort_thread(void* arg);

// Запуск демонстрационных примеров
void demonstrate_thread_creation();
void demonstrate_thread_joining();
void demonstrate_thread_parameters();
void demonstrate_thread_cancellation();
void demonstrate_thread_cleanup();
void demonstrate_sleepsort(int* array, int size);

#endif