#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int main() {
	srand(time(0));
	char num[50];
	int i, j, len;
	bool minusfl = false;
	int result = 0;
	int determ = 0;
	int lol = 1;

	printf("Enter matrix size: ");
	gets(num); // ввод значения размера матрицы
	len = strlen(num);
	for (i = 0; i < len; i++) {  // определение размера матрицы
		result = result * 10 + (num[i] - '0');
	}
	int* value = malloc(result * sizeof(int));
	int* resultat = malloc(result * sizeof(int));
	char* var = malloc((result * 2) * sizeof(char));
	int** arr = malloc(result * sizeof(int)); // определение размера матрицы
	for ( i = 0; i < result; i++) {  // определение вложеного размера матрицы
		arr[i] = malloc(result * sizeof(int));
	}
	for (int i = 0; i < result; i++) {
		value[i] = 0;
	}

	//arronelevelfeelzero(value, result);
	arrfeelzero(arr, result);
	//arrfeelrandom(arr, result);
	enterarr(arr, result, num, value, var);
	//arrprint(arr, result);
	//arronelevelprint(value, result);
	lol = slaucount(arr, result, resultat, value);
	if (lol == 1) {
		arronelevelcharprint(var, result, resultat);
	}
	//determ = determcount(arr, result);
	//printf("\nMatrix determinant = %d", determ);
}

int arrfeelzero(int *arr[], int result) { // заполнение массива 0
	for (int i = 0; i < result; i++) {
		for (int j = 0; j < result; j++) {
			arr[i][j] = 0;
		}
	}
	return arr;
}

int arronelevelfeelzero(int *value[], int result) { // заполнение массива 0
	for (int i = 0; i < result; i++) {
		value[i] = 0;
	}
	return value;
}

int arrfeelrandom(int *arr[], int result) { // Заполнение массива случайными значениями
	for (int i = 0; i < result; i++) {
		for (int j = 0; j < result; j++) {
			arr[i][j] = rand() % 100 + 1;
		}
	}
	return arr;
}

int slaucount(int* arr[], int result, int* resultat, int* value) { // заполнение массива 0
	int determ;
	determ = determcount(arr, result);
	if (determ == 0) {
		printf("No solutions");
		return 0;
	}
	int** valera = malloc(result * sizeof(int)); // определение размера матрицы
	for (int i = 0; i < result; i++) {  // определение вложеного размера матрицы
		valera[i] = malloc(result * sizeof(int));
	}
	for (int c = 0; c < result; c++) {
		for (int i = 0; i < result; i++) {
			for (int j = 0; j < result; j++) {
				valera[i][j] = arr[i][j];
			}
		}
		for (int i = 0; i < result; i++) {
			valera[i][c] = value[i];
		}
		resultat[c] = determcount(valera, result);
		resultat[c] = resultat[c] / determ;
	}
	return 1;
}

int determcount(int *arr[], int result) {  // Подсчёт определителя матрицы
	int majorline = 1;
	int minorline = 1;
	int determ = 0;
	int j, i;
	for (int c = 0; c < result; c++) { //количество диаганалей
		majorline = 1;
		minorline = 1;
		j = c; // отвечает за элемент столбца
		for (i = 0; i < result; i++) { //расчёт можора матрицы (i выступает как элемент столбца)
			majorline = majorline * arr[i][j];
			j++;
			if (j >= result) {
				j = 0;
			}
		}
		j = (result - 1) - c;
		for (i = 0; i < result; i++) { //расчёт минора матрицы (i выступает как элемент столбца)
			minorline = minorline *  arr[i][j];
			j--;
			if (j < 0) {
				j = result - 1;
			}
		}
		if (result > 2) {
			determ = determ + (majorline - minorline);
		}
		else if (result <= 2 && c == 0) {
			determ = determ + (majorline - minorline);
		}
	}
	return determ;
}

int enterarr(int *arr[], int result, char num[], int *value, char* var) { // ввод значений матрицы с клавиатуры
	int i, j, l, len;
	int v = 0;
	int d = 0;
	bool minusfl = false;
	bool qual = false;
	bool isvar = false;
	bool endqual = false;
	bool charinarr = false;
	bool nextnewvalue = false;
	printf("Enter your matrix (elements entering through space. Lines through new line): \n");
	for (i = 0; i < result; i++) {
		gets(num);
		len = strlen(num);
		j = 0;
		for (int c = 0; c < len; c++) {
			if (isalpha(num[c])) {
				for (l = 0; l < result; l++) {
					if (num[c] == var[l] || num[c] == var[0] && num[c + 1] == var[l + 3]) {
						if (j <= l) {
							if (!isdigit(num[c - 1])) {
								if (num[c - 1] == '-') {
									arr[i][j] = -1;
									minusfl = false;
									j++;
								}
								else {
									arr[i][j] = 1;
									j++;
								}
							}
							else {
								arr[i][l] = arr[i][j - 1];
								arr[i][j - 1] = 0;
								if (j == 1) {
									j = l + 1;
								}
								else {
									j = l;
								}
							}
						}
						charinarr = true;
					}
				}
				if (charinarr == false) {
					if (!isdigit(num[c - 1])) {
						if (num[c - 1] == '-') {
							arr[i][j] = -1;
							minusfl = false;
							j++;
						}
						else {
							arr[i][j] = 1;
							j++;
						}
					}
					var[d] = num[c];
					d++;
				}
				charinarr = false;
				isvar = true;
				nextnewvalue == true;
			}
			if (num[c] == '+') {
				isvar = false;
				nextnewvalue = true;
			}
			if (num[c] == '-' && isdigit(num[c+1])) {
				minusfl = true;
				isvar = false;
				nextnewvalue = true;
			}
			if (num[c] == '=') {
				isvar = false;
				qual = true;
			}
			if (isdigit(num[c])) {
				if (isvar == true) {
					var[d+2] = num[c];
					isvar = false;
					d++;
				}
				else if (qual == false) {
					arr[i][j] = arr[i][j] * 10 + (num[c] - '0');
					if (num[c + 1] == ' ' || num[c + 1] == NULL || isalpha(num[c+1])) {
						if (minusfl == true) {
							arr[i][j] = arr[i][j] * -1;
							minusfl = false;
						}
						nextnewvalue = false;
						j++;
					}
				}
				if (qual == true) {
					value[v] = value[v] * 10 + (num[c] - '0');
					if (num[c + 1] == ' ' || num[c + 1] == NULL) {
						if (minusfl == true) {
							value[v] = value[v] * -1;
							minusfl = false;
						}
						qual = false;
						v++;
					}
				}
			}
		}
	}
	return arr;
}

int arrprint(int *arr[], int result) {
	for (int i = 0; i < result; i++) {
		for (int j = 0; j < result; j++) {
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}
}

int arronelevelprint(int* value, int result) {
	for (int i = 0; i < result; i++) {
		printf("%d, ", value[i]);
	}
	printf("\n");
}

int arronelevelcharprint(char* var, int result, int* resultat) {
	for (int i = 0; i < result; i++) {
		if (var[i + (result/2)] == NULL) {
			printf("%c%c=%d \n", var[i], var[i + (result/2)], resultat[i]);
		}
		else {
			printf("%c=%d \n", var[i], resultat[i]);
		}
	}
	printf("\n");
}