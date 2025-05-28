#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define N 10000          // Размер массива
#define OUTPUT_LIMIT 20  // Ограничение на вывод первых 20 студентов

struct Student {
    char name[64];
    int math;
    int phy;
    int inf;
    int total;
};

void printCPUInfo() {
    system("wmic cpu get name, maxclockspeed");
}
// Создание студента
struct Student addStudent(const char* name, int math, int phy, int inf) {
    struct Student newStudent;
    strncpy(newStudent.name, name, 64);
    newStudent.math = math;
    newStudent.phy = phy;
    newStudent.inf = inf;
    newStudent.total = math + phy + inf;
    return newStudent;
}

// Вывод информации о студенте
void printStudentInfo(struct Student student) {
    printf("Name: %s, Math: %d, Physics: %d, Informatics: %d, Total: %d\n",
           student.name, student.math, student.phy, student.inf, student.total);
}

// Вывод первых limit студентов
void printStudents(struct Student students[], int size, int limit) {
    int printCount = (size < limit) ? size : limit;  // Выводим не более limit студентов
    for (int i = 0; i < printCount; i++) {
        printStudentInfo(students[i]);
    }
    if (size > limit) {
        printf("... (and %d more)\n", size - limit);
    }
}

// Заполнение массива студентов случайными данными
void fillStudents(struct Student students[], int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        char name[64];
        sprintf(name, "Student%d", i + 1);
        students[i] = addStudent(name, rand() % 101, rand() % 101, rand() % 101);
    }
}

// Insertion Sort
void insertionSort(struct Student students[], int size) {
    for (int i = 1; i < size; i++) {
        struct Student key = students[i];
        int j = i - 1;
        while (j >= 0 && students[j].total < key.total) {
            students[j + 1] = students[j];
            j = j - 1;
        }
        students[j + 1] = key;
    }
}

// Counting Sort
void countingSort(struct Student students[], int size) {
    int maxTotal = 300;
    int count[maxTotal + 1];
    struct Student* output = malloc(size * sizeof(struct Student));

    for (int i = 0; i <= maxTotal; i++) {
        count[i] = 0;
    }

    for (int i = 0; i < size; i++) {
        count[students[i].total]++;
    }

    for (int i = 1; i <= maxTotal; i++) {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        output[count[students[i].total] - 1] = students[i];
        count[students[i].total]--;
    }

    for (int i = 0; i < size; i++) {
        students[size-i-1] = output[i];
    }

    free(output);
}

// Quick Sort
void swap(struct Student* a, struct Student* b) {
    struct Student temp = *a;
    *a = *b;
    *b = temp;
}

int partition(struct Student students[], int low, int high) {
    int pivot = students[high].total;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (students[j].total > pivot) {
            i++;
            swap(&students[i], &students[j]);
        }
    }
    swap(&students[i + 1], &students[high]);
    return (i + 1);
}

void quickSort(struct Student students[], int low, int high) {
    if (low < high) {
        int pi = partition(students, low, high);
        quickSort(students, low, pi - 1);
        quickSort(students, pi + 1, high);
    }
}

// Замер времени
double getTime() {
    return (double)clock() / CLOCKS_PER_SEC;
}

int main() {
    // Выделяем память для массива студентов
    struct Student* students = malloc(N * sizeof(struct Student));
    if (students == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Заполняем массив студентов
    fillStudents(students, N);

    // Вывод первых 20 студентов до сортировки
    printf("Before sorting (first %d students):\n", OUTPUT_LIMIT);
    printStudents(students, N, OUTPUT_LIMIT);

    // Создаем резервную копию массива
    struct Student* studentsBackup = malloc(N * sizeof(struct Student));
    if (studentsBackup == NULL) {
        printf("Memory allocation failed!\n");
        free(students);
        return 1;
    }
    memcpy(studentsBackup, students, N * sizeof(struct Student));

    // Insertion Sort
    double start = getTime();
    insertionSort(students, N);
    double end = getTime();
    double insertionTime = end - start;
    printf("\nAfter Insertion Sort (first %d students):\n", OUTPUT_LIMIT);
    printStudents(students, N, OUTPUT_LIMIT);

    // Восстанавливаем массив из резервной копии
    memcpy(students, studentsBackup, N * sizeof(struct Student));

    // Counting Sort
    start = getTime();
    countingSort(students, N);
    end = getTime();
    double countingTime = end - start;
    printf("\nAfter Counting Sort (first %d students):\n", OUTPUT_LIMIT);
    printStudents(students, N, OUTPUT_LIMIT);

    // Восстанавливаем массив из резервной копии
    memcpy(students, studentsBackup, N * sizeof(struct Student));

    // Quick Sort
    start = getTime();
    quickSort(students, 0, N - 1);
    end = getTime();
    double quickTime = end - start;
    printf("\nAfter Quick Sort (first %d students):\n", OUTPUT_LIMIT);
    printStudents(students, N, OUTPUT_LIMIT);

    // Вывод времени выполнения всех сортировок
    printf("\nSorting Times:\n");
    printf("Insertion Sort: %f seconds\n", insertionTime);
    printf("Counting Sort: %f seconds\n", countingTime);
    printf("Quick Sort: %f seconds\n", quickTime);
    
    printCPUInfo();

    // Освобождаем память
    free(students);
    free(studentsBackup);

    return 0;
}