#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 1000

// Установка локали для корректной работы с русским языком
void init_locale() {
#ifdef _WIN32
    _wsetlocale(LC_ALL, L"Russian_Russia.65001");
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif
}

const wchar_t* get_error_message(int errnum) {
    static wchar_t wbuf[256];
    char buf[256];
    
#ifdef _WIN32
    strerror_s(buf, sizeof(buf), errnum);
#else
    strerror_r(errnum, buf, sizeof(buf));
#endif
    
    size_t ret = mbstowcs(wbuf, buf, sizeof(wbuf)/sizeof(wchar_t));
    if (ret == (size_t)-1) {
        return L"Неизвестная ошибка";
    }
    return wbuf;
}

int file_exists(const wchar_t *filename) {
#ifdef _WIN32
    struct _stat st;
    return _wstat(filename, &st) == 0;
#else
    struct stat st;
    char mb_filename[256];
    if (wcstombs(mb_filename, filename, sizeof(mb_filename)) == (size_t)-1) {
        return 0;
    }
    return stat(mb_filename, &st) == 0;
#endif
}

void reverse_wstring(wchar_t *str) {
    if (str == NULL || *str == L'\0') return;
    
    int length = wcslen(str);
    for (int i = 0; i < length / 2; i++) {
        wchar_t temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

void safe_remove_newline(wchar_t *str) {
    if (str == NULL || *str == L'\0') return;
    
    size_t len = wcslen(str);
    if (len > 0 && str[len - 1] == L'\n') {
        str[len - 1] = L'\0';
    }
}

FILE* open_input_file(const wchar_t *filename) {
#ifdef _WIN32
    FILE *file = _wfopen(filename, L"r,ccs=UTF-8");
#else
    char mb_filename[256];
    wcstombs(mb_filename, filename, sizeof(mb_filename));
    FILE *file = fopen(mb_filename, "r");
#endif
    
    if (file == NULL) {
        fwprintf(stderr, L"Ошибка открытия файла '%ls': %ls\n", 
                filename, get_error_message(errno));
    }
    return file;
}

FILE* open_output_file(const wchar_t *filename) {
#ifdef _WIN32
    FILE *file = _wfopen(filename, L"w,ccs=UTF-8");
#else
    char mb_filename[256];
    wcstombs(mb_filename, filename, sizeof(mb_filename));
    FILE *file = fopen(mb_filename, "w");
#endif
    
    if (file == NULL) {
        fwprintf(stderr, L"Ошибка открытия файла '%ls': %ls\n", 
                filename, get_error_message(errno));
    }
    return file;
}

void create_sample_file(const wchar_t *filename) {
    FILE *file = open_output_file(filename);
    if (file == NULL) return;
    
    // Добавляем BOM для Windows
    fputwc(0xFEFF, file);
    
    const wchar_t *sample_lines[] = {
        L"Первая строка в файле",
        L"Вторая строка текста",
        L"Пример работы программы",
        L"Реверс строк в файле",
        L"STOP"
    };
    
    for (size_t i = 0; i < sizeof(sample_lines)/sizeof(sample_lines[0]); i++) {
        fputws(sample_lines[i], file);
        fputws(L"\n", file);
    }
    
    fclose(file);
    wprintf(L"Создан файл '%ls' с примером данных.\n", filename);
}

int process_file(const wchar_t *input_filename, const wchar_t *output_filename) {
    if (!file_exists(input_filename)) {
        wprintf(L"Файл '%ls' не существует. Создать? (y/n): ", input_filename);
        
        wint_t answer = fgetwc(stdin);
        if (answer == L'y' || answer == L'Y') {
            create_sample_file(input_filename);
        } else {
            wprintf(L"Отменено пользователем.\n");
            return -1;
        }
    }

    FILE *input_file = open_input_file(input_filename);
    if (input_file == NULL) return -1;

    FILE *output_file = open_output_file(output_filename);
    if (output_file == NULL) {
        fclose(input_file);
        return -1;
    }

    wchar_t line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (fgetws(line, MAX_LINE_LENGTH, input_file) != NULL) {
        safe_remove_newline(line);
        reverse_wstring(line);
        
        if (fputws(line, output_file) == WEOF) {
            fwprintf(stderr, L"Ошибка записи в файл '%ls'\n", output_filename);
            break;
        }
        fputws(L"\n", output_file);
        line_count++;
    }

    if (ferror(input_file)) {
        fwprintf(stderr, L"Ошибка чтения файла '%ls'\n", input_filename);
    }

    fclose(input_file);
    fclose(output_file);

    wprintf(L"Обработано %d строк. Результат записан в '%ls'.\n", 
           line_count, output_filename);
    return line_count;
}

int input_strings_interactive(const wchar_t *output_filename) {
    FILE *output_file = open_output_file(output_filename);
    if (output_file == NULL) return -1;

    // Добавляем BOM для Windows
    fputwc(0xFEFF, output_file);

    wprintf(L"Введите строки (для завершения введите 'STOP' или Ctrl+Z):\n");

    wchar_t line[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (1) {
        if (fgetws(line, MAX_LINE_LENGTH, stdin) == NULL) {
            if (feof(stdin)) {
                wprintf(L"Обнаружен конец ввода.\n");
                break;
            }
            fwprintf(stderr, L"Ошибка ввода: %ls\n", get_error_message(errno));
            break;
        }

        safe_remove_newline(line);

        if (wcscmp(line, L"STOP") == 0) {
            wprintf(L"Обнаружено стоп-слово.\n");
            break;
        }

        reverse_wstring(line);

        if (fputws(line, output_file) == WEOF) {
            fwprintf(stderr, L"Ошибка записи в файл '%ls'\n", output_filename);
            break;
        }
        fputws(L"\n", output_file);
        line_count++;
    }

    fclose(output_file);
    
    if (line_count > 0) {
        wprintf(L"Сохранено %d строк в файл '%ls'.\n", line_count, output_filename);
    }
    
    return line_count;
}

void print_usage(const wchar_t *program_name) {
    wprintf(L"Использование:\n");
    wprintf(L"  %ls input.txt output.txt  - реверс строк из файла\n", program_name);
    wprintf(L"  %ls -i output.txt        - интерактивный ввод\n", program_name);
    wprintf(L"  %ls                      - создание примеров файлов\n", program_name);
}

int main(int argc, wchar_t *argv[]) {
    init_locale();

    if (argc == 1) {
        const wchar_t *input_file = L"input.txt";
        const wchar_t *output_file = L"output.txt";
        
        if (!file_exists(input_file)) {
            create_sample_file(input_file);
        }
        
        process_file(input_file, output_file);
        return EXIT_SUCCESS;
    }
    else if (argc == 3 && wcscmp(argv[1], L"-i") != 0) {
        return process_file(argv[1], argv[2]) >= 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else if (argc == 3 && wcscmp(argv[1], L"-i") == 0) {
        return input_strings_interactive(argv[2]) >= 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
}