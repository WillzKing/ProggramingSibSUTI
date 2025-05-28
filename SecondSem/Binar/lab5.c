#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME 100
#define MAX_PHONE 20
#define MAX_ADDRESS 100
#define FILENAME "Abonent.dat"

typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char address[MAX_ADDRESS];
} Abonent;

void createFile() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Ошибка при создании файла.\n");
        exit(1);
    }
    
    int N;
    printf("Введите количество абонентов: ");
    scanf("%d", &N);
    getchar(); // очистка буфера
    
    for (int i = 0; i < N; i++) {
        Abonent abonent;
        printf("\nАбонент #%d:\n", i+1);
        
        printf("ФИО: ");
        fgets(abonent.name, MAX_NAME, stdin);
        abonent.name[strcspn(abonent.name, "\n")] = '\0'; // удаляем символ новой строки
        
        printf("Номер телефона: ");
        fgets(abonent.phone, MAX_PHONE, stdin);
        abonent.phone[strcspn(abonent.phone, "\n")] = '\0';
        
        printf("Адрес: ");
        fgets(abonent.address, MAX_ADDRESS, stdin);
        abonent.address[strcspn(abonent.address, "\n")] = '\0';
        
        fwrite(&abonent, sizeof(Abonent), 1, file);
    }
    
    fclose(file);
    printf("\nФайл %s успешно создан с %d записями.\n\n", FILENAME, N);
}

void searchInFile() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("Файл не найден. Сначала создайте файл.\n");
        return;
    }
    
    int choice;
    printf("\nПо какому полю выполнить поиск?\n");
    printf("1. По ФИО\n");
    printf("2. По номеру телефона\n");
    printf("3. По адресу\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);
    getchar(); // очистка буфера
    
    char searchTerm[100];
    printf("Введите строку для поиска: ");
    fgets(searchTerm, 100, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    
    Abonent abonent;
    int found = 0;
    
    printf("\nРезультаты поиска:\n");
    while (fread(&abonent, sizeof(Abonent), 1, file)) {
        int match = 0;
        
        switch (choice) {
            case 1:
                if (strcmp(abonent.name, searchTerm) == 0) match = 1;
                break;
            case 2:
                if (strcmp(abonent.phone, searchTerm) == 0) match = 1;
                break;
            case 3:
                if (strcmp(abonent.address, searchTerm) == 0) match = 1;
                break;
            default:
                printf("Неверный выбор.\n");
                fclose(file);
                return;
        }
        
        if (match) {
            printf("\nФИО: %s\n", abonent.name);
            printf("Телефон: %s\n", abonent.phone);
            printf("Адрес: %s\n", abonent.address);
            found = 1;
        }
    }
    
    if (!found) {
        printf("Записи не найдены.\n");
    }
    
    fclose(file);
}

int main() {
    int choice;
    
    do {
        printf("1. Создать файл с абонентами\n");
        printf("2. Поиск абонентов\n");
        printf("3. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);
        getchar(); // очистка буфера
        
        switch (choice) {
            case 1:
                createFile();
                break;
            case 2:
                searchInFile();
                break;
            case 3:
                printf("Выход...\n");
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 3);
    
    return 0;
}