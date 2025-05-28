#include <stdio.h>
#include <string.h>
#include "phonebook.h"

Contact phonebook[MAX_RECORDS] = {
    {"Иванов И.И.", "+79161234567", "ул. Ленина, 10, кв.5", "ivanov@mail.ru"},
    {"Петров П.П.", "+79031234568", "пр. Мира, 15, кв.12", "petrov@gmail.com"},
    {"Сидоро А.С.", "+79261234569", "ул. Гагарина, 3     ", "sidoro@yandex.ru"},
    {"Кузнец А.В.", "+79091234570", "ул. Советская, 22   ", "kuznet@mail.ru"},
    {"Смирно О.Д.", "+79171234571", "пр. Победы, 45", "smirno@gmail.com"}
};

void print_contact(Contact c) {
    printf("│ %-28s │ %-15s │ %-28s │ %-25s │\n", 
           c.name, c.phone, c.address, c.email);
}

void print_table_header() {
    printf("\n┌──────────────────────────────┬─────────────────┬──────────────────────────────┬───────────────────────────┐\n");
    printf("│ %-28s │ %-15s │ %-28s │ %-25s │\n", "ФИО", "Телефон", "Адрес", "Email");
    printf("├──────────────────────────────┼─────────────────┼──────────────────────────────┼───────────────────────────┤\n");
}

void print_table_footer() {
    printf("└──────────────────────────────┴─────────────────┴──────────────────────────────┴───────────────────────────┘\n");
}

void print_phonebook() {
    printf("\nТелефонный справочник:");
    print_table_header();
    for (int i = 0; i < MAX_RECORDS; i++) {
        print_contact(phonebook[i]);
    }
    print_table_footer();
}