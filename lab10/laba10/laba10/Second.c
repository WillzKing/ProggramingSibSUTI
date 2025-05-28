#include "Second.h"

struct Line {
	int rateA;
	int rateB;
};

int imput(int HowMuchLines, struct Line* lines) {
	for (int i = 0; i < HowMuchLines; i++) {
		printf("Enter rate A for %d direct: ", i + 1);
		lines[i].rateA = ImputIntScan();
		printf("Enter rate B for %d direct: ", i + 1);
		lines[i].rateB = ImputIntScan();
		printf("\n");
	}
	printf("\n");
}

int ImputIntScan() {
	int a = 0;
	char line[50];
	gets(line);
	int len = strlen(line);
	for (int i = 0; i < len; i++) {
		a = a * 10 + (line[i] - '0');
	}
	return a;
}

int HowMuchParallels(int HowMuchLines, struct Line* lines) {
	int ForOx = 0;
	int ForOy = 0;
	for (int i = 0; i < HowMuchLines; i++) {
		if (lines[i].rateA == 0) {
			ForOx++;
		}
		if (lines[i].rateB == 0) {
			ForOy++;
		}
	}
	printf("Parallel lines for Oy axis = %d\nParallel lines for Ox axis = %d\n", ForOx, ForOy);
}

int OutLines(int a, struct Line* lines) {
	for (int i = 0; i < a; i++) {
		printf("%d direct have %d A rate and %d B rate \n", i + 1, lines[i].rateA, lines[i].rateB);
	}
}