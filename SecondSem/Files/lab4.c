#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_LINE_LENGTH 1000

// Функция для реверса строки
void reverse_string(char *str) {
    if (str == NULL) return;
    
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

// Функция для обработки файла
void process_existing_file(const char *input_filename, const char *output_filename) {
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Ошибка открытия файла '%s' для чтения.\n", input_filename);
        return;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Ошибка открытия файла '%s' для записи.\n", output_filename);
        fclose(input_file);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        // Убираем символ новой строки
        line[strcspn(line, "\n")] = '\0';

        // Реверсируем строку
        reverse_string(line);

        // Записываем реверсированную строку в файл
        fprintf(output_file, "%s\n", line);
    }

    fclose(input_file);
    fclose(output_file);

    printf("Файл '%s' успешно обработан. Результат в '%s'.\n", input_filename, output_filename);
}

// Функция для ввода строк через консоль
void input_strings_until_stop(const char *output_filename) {
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Ошибка открытия файла '%s' для записи.\n", output_filename);
        return;
    }

    printf("Введите строки (для завершения введите 'STOP'):\n");

    char line[MAX_LINE_LENGTH];
    while (1) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) {
            fprintf(stderr, "Ошибка ввода строки.\n");
            break;
        }

        // Убираем символ новой строки
        line[strcspn(line, "\n")] = '\0';

        // Проверяем на стоп-слово
        if (strcmp(line, "STOP") == 0) {
            break;
        }

        // Реверсируем строку
        reverse_string(line);

        // Записываем реверсированную строку в файл
        fprintf(output_file, "%s\n", line);
    }

    fclose(output_file);

    printf("Строки записаны в файл '%s'.\n", output_filename);
}

void print_usage(const char *program_name) {
    printf("Использование:\n");
    printf("  %s input.txt output.txt  - реверс строк из файла input.txt в output.txt\n", program_name);
    printf("  %s -i output.txt        - интерактивный ввод с сохранением в output.txt\n", program_name);
}

int main(int argc, char *argv[]) {
    // Устанавливаем локаль для корректной работы с русским языком
    setlocale(LC_ALL, "Russian_Russia.1251"); // Для Windows (CP1251)
    // setlocale(LC_ALL, "ru_RU.UTF-8"); // Для Linux

    // Проверка количества аргументов
    if (argc < 2) {
        fprintf(stderr, "Ошибка: недостаточно аргументов.\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Режим обработки файла
    if (argc == 3 && strcmp(argv[1], "-i") != 0) {
        process_existing_file(argv[1], argv[2]);
    }
    // Интерактивный режим
    else if (argc == 3 && strcmp(argv[1], "-i") == 0) {
        input_strings_until_stop(argv[2]);
    }
    // Неправильные аргументы
    else {
        fprintf(stderr, "Ошибка: неверные аргументы.\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}