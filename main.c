#include <stdlib.h>
#include <stdio.h>

#define GRIDSIZE 9 * 9 // shoudnt be changed

unsigned int * grid;

void print() {

	unsigned int i = 0;
	for (unsigned int y = 0; y < 9; ++y) {
		for (unsigned int x = 0; x < 9; ++x) {
	
			if (x != 0 && x % 3 == 0) {
				putchar(' ');
			}

			printf("%u ", grid[i]);

			i++;
		}
		if (y != 0 && (y + 1) % 3 == 0) {
			putchar('\n');
			for (unsigned int j = 0; j < 9 + 9 + 1; ++j) {
				if (j % 6 == 0) putchar('+');
				else            putchar('-');
			}
		}
		putchar('\n');
	}


}

int main(void) {

	grid = malloc((
		GRIDSIZE
	) * sizeof(unsigned int));

	for (unsigned int i = 0; i < GRIDSIZE; ++i) {
		grid[i] = (i + 1 + (i / 9)) % 9 ;
		if (grid[i] == 0) grid[i] = 9;
	}

	print();

	return 0;
}