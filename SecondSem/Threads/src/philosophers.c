#include "../include/philosophers.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// Dlya kross-platformennoy podderzhki sleep/usleep
#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep((x) * 1000)
#define usleep(x) Sleep((x) / 1000)
#else
#include <unistd.h>
#endif

// Global'nye peremennye
pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];
int philosopher_ids[NUM_PHILOSOPHERS];
bool running = true;

// Funktsiya dlya vzyatiya vilok
void pick_forks(int philosopher_id) {
    int left_fork = philosopher_id;
    int right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;
    
    // Chetные filosofy berut snachala levuyu vilku
    // Nechetные - snachala pravuyu (dlya predotvrashcheniya deadlock)
    if (philosopher_id % 2 == 0) {
        pthread_mutex_lock(&forks[left_fork]);
        printf("Filosof %d vzyal levuyu vilku %d\n", philosopher_id, left_fork);
        pthread_mutex_lock(&forks[right_fork]);
        printf("Filosof %d vzyal pravuyu vilku %d\n", philosopher_id, right_fork);
    } else {
        pthread_mutex_lock(&forks[right_fork]);
        printf("Filosof %d vzyal pravuyu vilku %d\n", philosopher_id, right_fork);
        pthread_mutex_lock(&forks[left_fork]);
        printf("Filosof %d vzyal levuyu vilku %d\n", philosopher_id, left_fork);
    }
}

// Funktsiya dlya osvobozhdeniya vilok
void release_forks(int philosopher_id) {
    int left_fork = philosopher_id;
    int right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;
    
    pthread_mutex_unlock(&forks[left_fork]);
    pthread_mutex_unlock(&forks[right_fork]);
    
    printf("Filosof %d osvobodil vilki %d i %d\n", 
           philosopher_id, left_fork, right_fork);
}

// Funktsiya filosofа
void* philosopher(void* num) {
    int philosopher_id = *(int*)num;
    
    while (running) {
        // Filosof razmyshlyaet
        printf("Filosof %d razmyshlyaet...\n", philosopher_id);
        sleep(1); // Ispol'zuyem kross-platformennyy sleep
        
        // Pytayetsya vzyat' vilki
        printf("Filosof %d khochet est'\n", philosopher_id);
        pick_forks(philosopher_id);
        
        // Filosof est
        printf("Filosof %d est\n", philosopher_id);
        usleep(EATING_TIME); // Ispol'zuyem kross-platformennyy usleep
        
        // Osvobozhdaet vilki
        release_forks(philosopher_id);
    }
    
    return NULL;
}

// Initsializatsiya vilok
void init_forks() {
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
}

// Ochistka vilok
void cleanup_forks() {
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
}

// Osnovnaya funktsiya
void dine_philosophers() {
    printf("\n=== Zadacha obedayushchikh filosofov ===\n");
    printf("Ispol'zuetsya strategiya izbezhaniya deadlock'ov:\n");
    printf("- Chetные filosofy berut snachala levuyu vilku\n");
    printf("- Nechetные filosofy berut snachala pravuyu vilku\n\n");
    
    srand(time(NULL));
    init_forks();
    
    // Sozdaem potoki filosofov
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }
    
    // Daem filosofam pouzhinat' 10 sekund
    sleep(10);
    
    // Zavershaem rabotu
    running = false;
    printf("\nZavershaem uzhin...\n");
    
    // Ozhidaem zaversheniya vsekh potokov
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }
    
    cleanup_forks();
}
