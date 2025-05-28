#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#define NUM_PHILOSOPHERS 5
#define EATING_TIME 100000
#define THINKING_TIME 100000

void dine_philosophers();
void* philosopher(void* num);

#endif