#include <stdio.h>
#include <string.h>
#include "phonebook.h"
#include "sort.h"

void sort_by_name(int index_array[]) {
    for (int i = 0; i < MAX_RECORDS - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < MAX_RECORDS; j++) {
            if (strcmp(phonebook[index_array[j]].name, 
                      phonebook[index_array[min_idx]].name) < 0) {
                min_idx = j;
            }
        }
        int temp = index_array[i];
        index_array[i] = index_array[min_idx];
        index_array[min_idx] = temp;
    }
}

void sort_by_phone(int index_array[]) {
    for (int i = 1; i < MAX_RECORDS; i++) {
        int key = index_array[i];
        int j = i - 1;
        while (j >= 0 && strcmp(phonebook[index_array[j]].phone, 
                               phonebook[key].phone) > 0) {
            index_array[j + 1] = index_array[j];
            j = j - 1;
        }
        index_array[j + 1] = key;
    }
}

void print_phonebook_sorted(int index_array[], const char* sort_type) {
    printf("\nТелефонный справочник (%s):", sort_type);
    print_table_header();
    for (int i = 0; i < MAX_RECORDS; i++) {
        print_contact(phonebook[index_array[i]]);
    }
    print_table_footer();
}