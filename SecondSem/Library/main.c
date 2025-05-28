#include <stdio.h>
#include "phonebook.h"
#include "sort.h"

int main() {
    int name_index[MAX_RECORDS];
    int phone_index[MAX_RECORDS];
    
    // Инициализация индексных массивов
    for (int i = 0; i < MAX_RECORDS; i++) {
        name_index[i] = i;
        phone_index[i] = i;
    }
    
    print_phonebook();
    
    sort_by_name(name_index);
    print_phonebook_sorted(name_index, "отсортировано по ФИО");
    
    sort_by_phone(phone_index);
    print_phonebook_sorted(phone_index, "отсортировано по телефону");
    
    return 0;
}