#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Second.h"

struct Line {
	int rateA;
	int rateB;
};


int main() {
	int HowMuchLines = 0;
	printf("U askink only rates A and B in direct equations y=ax+b \n");
	printf("Enter how much directs u wanna enter: \n");
	HowMuchLines = ImputIntScan();
	struct Line* lines = malloc(HowMuchLines * sizeof(struct Line));
	
	imput(HowMuchLines, &lines);
	//OutLines(HowMuchLines, &lines);
	HowMuchParallels(HowMuchLines, &lines);

	free(lines);
}