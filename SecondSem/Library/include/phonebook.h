#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#define MAX_RECORDS 5
#define MAX_NAME 30
#define MAX_PHONE 16
#define MAX_ADDRESS 40
#define MAX_EMAIL 25

typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char address[MAX_ADDRESS];
    char email[MAX_EMAIL];
} Contact;

extern Contact phonebook[MAX_RECORDS];

// Объявляем функции для работы с таблицей
void print_contact(Contact c);
void print_table_header();
void print_table_footer();
void print_phonebook();

#endif