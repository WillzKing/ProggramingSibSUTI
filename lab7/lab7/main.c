#include <stdio.h>

int main() {
	char text[] = "Vvedite text s tochkoi v conce.";
	int iflt = 0;
	int illt = 0;
	char flt = text[0];
	char llt = ' ';
	for (int i = 0; text[i] != NULL; i++) {
		if (text[i] == ' ' || text[i] == '.') {
			llt = text[i - 1];
			illt = i - 1;
			text[iflt] = llt;
			text[illt] = flt;
			iflt = i + 1;
			flt = text[i + 1];
		}
	}
	for (int i = 0; text[i] != NULL; i++) {
		printf("%c", text[i]);
	}
}