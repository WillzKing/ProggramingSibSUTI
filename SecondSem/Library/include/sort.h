#ifndef SORT_H
#define SORT_H

#include "phonebook.h"  // Для типа Contact

// Объявляем функции сортировки
void sort_by_name(int index_array[]);
void sort_by_phone(int index_array[]);
void print_phonebook_sorted(int index_array[], const char* sort_type);

#endif