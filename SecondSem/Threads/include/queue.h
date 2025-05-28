#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

#define MAX_MSG_LENGTH 128
#define MAX_QUEUE_SIZE 15

typedef struct {
    char message[MAX_MSG_LENGTH];
} QueueItem;

typedef struct {
    QueueItem items[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} MessageQueue;

// Инициализация и очистка очереди
void init_queue(MessageQueue* queue);
void cleanup_queue(MessageQueue* queue);

// Основные операции
int msgSend(MessageQueue* queue, const char* msg);
int msgRecv(MessageQueue* queue, char* buf, size_t bufsize);
int msgDrop(MessageQueue* queue);

#endif