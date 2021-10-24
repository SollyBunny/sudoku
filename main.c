#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <time.h>
#define digittochar(a) (a==0) ? (' ') : (a+48)

// #define false 0
// #define true  1
// typedef int bool;

// Key codes
#define BREAK 3

#define KEYW 119
#define KEYA 97
#define KEYS 115
#define KEYD 100

#define KEY0 48

#define GRIDSIZE (9 * 9) // shoudnt be changed

struct Tile {
	unsigned int val;
	unsigned int use;
};

struct Tile grid[GRIDSIZE];
unsigned int tempgrid[GRIDSIZE];
unsigned int tilesleft;

char gridtxt[] = "                           \n╔═══════╦═══════╦═══════╗\n║ A A A ║ A A A ║ A A A ║\n║ A A A ║ A A A ║ A A A ║\n║ A A A ║ A A A ║ A A A ║\n╠═══════╬═══════╬═══════╣\n║ A A A ║ A A A ║ A A A ║\n║ A A A ║ A A A ║ A A A ║\n║ A A A ║ A A A ║ A A A ║\n╠═══════╬═══════╬═══════╣\n║ A A A ║ A A A ║ A A A ║\n║ A A A ║ A A A ║ A A A ║\n║ A A A ║ A A A ║ A A A ║\n╚═══════╩═══════╩═══════╝\n                           ";
unsigned int gridpos[] = {28, 30, 32, 36, 38, 40, 44, 46, 48, 54, 56, 58, 62, 64, 66, 70, 72, 74, 80, 82, 84, 88, 90, 92, 96, 98, 100, 132, 134, 136, 140, 142, 144, 148, 150, 152, 158, 160, 162, 166, 168, 170, 174, 176, 178, 184, 186, 188, 192, 194, 196, 200, 202, 204, 236, 238, 240, 244, 246, 248, 252, 254, 256, 262, 264, 266, 270, 272, 274, 278, 280, 282, 288, 290, 292, 296, 298, 300, 304, 306, 308};
#define gridx 26
#define gridy 13

unsigned int cursorpos;

/*char digittochar(unsigned int i) {
	switch (i) {
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
	}
	return 'E';
	return 48 + i; // this really works lmao
}*/

// util function to get base 10 intiger "size"
unsigned int intsize(unsigned int a) {
	if      (a < 10       ) return 1;
	else if (a < 100      ) return 2;
	else                    return 3;
}

int checkgrid() {
	//return 1;

	//unsigned int sum = grid[0] * 3;

	// i = row
	// m = col
	// j = block blk

	unsigned int row[9];
	unsigned int col[9];
	unsigned int blk[9];
	
	for (unsigned int i = 0; i < 9; ++i) {

		for (unsigned int m = 0; m < 9; ++m) {
			/*printf("row: %u, col: %u, blk: %u, row: %u, col: %u, blk: %u\n", 
				(i * 9) + m, 
				(m * 9) + i, 
				((i * 3) % 9) + (i / 3 * 9 * 3) + (m % 3) + (m / 3 * 9),
				grid[(i * 9) + m],
				grid[(m * 9) + i],
				grid[((i * 3) % 9) + (i / 3 * 9 * 3) + (m % 3) + (m / 3 * 9)]
			);*/ // DEBUG
			row[m] = grid[(i * 9) + m].val;
			col[m] = grid[(m * 9) + i].val;
			blk[m] = grid[((i * 3) % 9) + (i / 3 * 9 * 3) + (m % 3) + (m / 3 * 9)].val;

			for (unsigned int _ = 0; _ < m; ++_) { // only have to compare upto whatve been set
				if ((row[_] == row[m]) && (row[_] != 0)) return 0;
				if ((col[_] == col[m]) && (col[_] != 0)) return 0;
				if ((blk[_] == blk[m]) && (blk[_] != 0)) return 0;
			}

			// m += 9;
			// j++;
			// if (j % 3 == 0) {
				// j += 9 - 3;
			// }
		}

		
		
		
		// m = i / 9;
		// j = ((m * 3) % 9) + (m / 3 * 9 * 3);
		//printf("J DONE: %u\n", j);

	}

	
	// unsigned int m = 9;
	// unsigned int j = 1;
	// for (unsigned int i = 1; i < GRIDSIZE; ++i) {
// 
		// if (i % 9 == 0) {
// 
			// for (unsigned int _ = 0; _ < 9; ++_) {
				// row[_] = 0;
				// col[_] = 0;
				// blk[_] = 0;
			// }
			// 
			// /*if (sum <= 135) { // 135
				// //printf("FAIL: %u %u %u = %u\n", m, i, j, sum);
				// return 0;
			// }*/
// 
			// //sum = 0;
			// 
			// m = i / 9;
			// j = ((m * 3) % 9) + (m / 3 * 9 * 3);
			// //printf("J DONE: %u\n", j);
		// }
// 
		// //printf("j: %uA %uB\n", j, grid[j]);
		// /*sum += grid[i];
		// sum += grid[m];
		// sum += grid[j];*/
// 
		// printf("%u %u %u\n", i, m, j);
		// // check if item is already in list
		// for (unsigned int _ = 0; _ < 9; ++_) {
			// //if (grid[i] == 0) {putchar('i');continue;}
			// printf("%u == %u is %u\n", row[_], grid[i], row[_] ==  grid[i]);
			// if (row[_] == grid[i]) { printf("OH FUCK OH FUCK OH FUCK"); return 0; };
		// }
		// row[i / 9] = grid[i];
		// putchar('\n');
		// 
		// for (unsigned int _ = 0; _ < 9; ++_) {
			// //if (grid[m] == 0) {putchar('m');continue;}
			// printf("%u == %u is %u\n", col[_], grid[m], col[_] ==  grid[m]);
			// if (col[_] == grid[m]) { printf("OH FUCK OH FUCK OH FUCK"); return 0; };
		// }
		// col[i / 9] = grid[m];
		// putchar('\n');
// 
		// for (unsigned int _ = 0; _ < 9; ++_) {
			// //if (grid[j] == 0) {putchar('j');continue;}
			// printf("%u == %u is %u\n", blk[_], grid[j], blk[_] ==  grid[j]);
			// if (blk[_] == grid[j]) { printf("OH FUCK OH FUCK OH FUCK"); return 0; };
		// }
		// blk[i / 9] = grid[j];
		// putchar('\n');
// 
		// 
		// putchar('\n');
// 
		// m += 9;
		// j++;
		// if (j % 3 == 0) {
			// j += 9 - 3;
		// }
// 
	// }*/
		// sum = 0;
		// for (unsigned int x = 0; x < 9; ++x) {
			// sum += grid[m];
			// sum += grid[i];
			// i++;
			// m+=9;
		// }
		// if (sum != 90) {
			// printf("%u %u\n", sum, sum);
			// return 0;
		// }
		// m++;
	// }

	return 1;
	
}

void print() {

	// i know this may be ineffecient but it makes the code so much more readable x-x

	char * i;
	struct Tile * m;

	/*unsigned int j = 0;
	for (i = gridtxt; *i != '\n'; ++i) {
		j++;
	}
	printf("\x1b[%uC", j / 14);*/

	m = grid;
	for (i = gridtxt; *i; ++i) {
		if (*i == 'A') {
			putchar(digittochar((*m).val));
			++m;
		} else if (*i == '\n') {
			putchar(' ');
			putchar(' ');
			putchar('\n');
			putchar(' ');
			putchar(' ');
		} else {
			putchar(*i);
		}
	}

	/*fputs("+-------+-------+-------+\n| ", stdout);

	unsigned int i = 0;
	for (unsigned int y = 0; y < 9; ++y) {
		for (unsigned int x = 0; x < 9; ++x) {
	
			if (x != 0 && x % 3 == 0) {
				putchar('|');
				putchar(' ');
			}

			printf("%u ", grid[i]);

			i++;
		}
		putchar('|');
		putchar('\n');
		if (y != 0 && (y + 1) % 3 == 0) {

			putchar('+');
			for (unsigned int j = 0; j < 9 * 2 + 5; ++j) {
				if ((j + 1) % 8 == 0) putchar('+');
				else                  putchar('-');
			}
			putchar('+');
			putchar('\n');
			
		} else {

			
			
		}

		putchar('|');
					putchar(' ');
	}*/


}

int main(void) {

	/*grid = malloc((
		GRIDSIZE
	) * sizeof(unsigned int));*/

	srand(time(0));

	// set terminal settins (dont echo characters / queue stdin / ignore ctrl-c )
	struct termios term, restore;
	tcgetattr(0, &term);
	tcgetattr(0, &restore); // backup the original terminal state to restore later
	term.c_lflag &= ~(ICANON|ECHO|ISIG);
	tcsetattr(0, TCSANOW, &term);

	printf("\x1b[2J\x1b[48;5;237m\x1b[?25l\x1b[H\x1b[1m\x1b[7m");
	
	for (unsigned int i = 0; i < GRIDSIZE; ++i) {
		grid[i].val = (
			i + 1 + // inital 1-9 pattern
			(i / 9 * 3) + // offset 3 per row
			(i / (9 * 3)) // offset 9 per large row
		) % 9 + 1;
		//grid[i].use = 0;
		//if (grid[i].val == 0) grid[i].val = 9;
	}

	/*unsigned int i = 0;
	for (unsigned int y = 1; y < 9 + 1; ++y) {
		for (unsigned int x = 1; x < 9 + 1; ++x) {
			grid[i] = (x + y) % 9 + 1;
			i++;
		}	
	}*/

	/*unsigned int temp = 0;
	unsigned int r;

	for (unsigned int i = 0; i < GRIDSIZE; ++i) {

		temp = 0;
		do {
		
			if (temp == 10) {
				for (unsigned int _ = 0; _ < 10; _++) {
					grid[rand() % GRIDSIZE + 1] = rand() % 9 + 1;
				}
				temp = 0;
			};
			temp++;
			r = rand() % 9 + 1;
			grid[i] = r;
			
		} while (0 == checkgrid());
		
	}*/

	//board = [[(i + k) % 9 + 1 for i in range(1, height + 1)] for k in range(width)]
	
	unsigned int temp;
	unsigned int r1;
	unsigned int r2;

	for (unsigned int i = 0; i < 20; ++i) {
		gridgenloopstart:

		for (unsigned int j = 0; j < GRIDSIZE; ++j) {
			tempgrid[j] = grid[j].val;
		}
		if (rand() % 2 < 1) {
			r1 = (rand() % 9) * 9;
			r2 = (rand() % 9) * 9;
			for (unsigned int m = 0; m < 9; ++m) {
				temp = grid[r1 + m].val;
				grid[r1 + m].val = grid[r2 + m].val;
				grid[r2 + m].val = temp; 
			}
		} else {
			r1 = (rand() % 9);
			r2 = (rand() % 9);
			for (unsigned int m = 0; m < 9; ++m) {
				temp = grid[r1 + (m * 9)].val;
				grid[r1 + (m * 9)].val = grid[r2 + (m * 9)].val;
				grid[r2 + (m * 9)].val = temp; 
			}
		}

		if (!checkgrid()) {
			for (unsigned int j = 0; j < GRIDSIZE; ++j) {
				grid[j].val = tempgrid[j];
			}
			goto gridgenloopstart;
		}
		
	}

	tilesleft = 0;
	for (unsigned int i = 0; i < GRIDSIZE / 2; ++i) {
		r1 = rand() % GRIDSIZE;
		grid[r1].val = 0;
		grid[r1].use = 1;
		tilesleft++;
	}

	print();
	printf("\x1b[22m\x1b[0;%uHTiles Left: %u", 
		(31 - 12 - intsize(tilesleft)) / 2, 
		tilesleft
	);
	//goto end;

	cursorpos = GRIDSIZE / 2;
	while (grid[cursorpos].use == 0) {
		cursorpos++;
	}

	char c;

	goto mainloopcontinue;

	while ((c = getchar()) != BREAK) {

		// clear previous cursor
		printf("\x1b[7m\x1b[%u;%uH%c", (gridpos[cursorpos] / gridx) + 2, (gridpos[cursorpos] % gridx) + 3, digittochar(grid[cursorpos].val));

		mainloopkeys:
			switch (c) {

				case KEYW:
					cursorpos -= 9;
					if (cursorpos > GRIDSIZE) {
						cursorpos += GRIDSIZE;
					}
					break;
					
				case KEYS:
					cursorpos += 9;
					if (cursorpos > GRIDSIZE) {
						cursorpos -= GRIDSIZE;
					}
					break;
					
				case KEYA:
					if (cursorpos % 9 == 0) {
						cursorpos += 9;
					}
					cursorpos--;
					break;
					
				case KEYD:
					if (cursorpos % 9 == 9 - 1) {
						cursorpos -= 9;
					}
					cursorpos++;
					break;

				case KEY0:
					if (grid[cursorpos].val == 0) break;
					grid[cursorpos].val = 0;
					tilesleft++;
					printf("\x1b[0;%uHTiles Left: %u", 
						(31 - 12 - intsize(tilesleft)) / 2, 
						tilesleft
					);
					break;

				default:

					if (c > KEY0 && c <= KEY0 + 9) {

						temp = grid[cursorpos].val;
						grid[cursorpos].val = c - KEY0;	
						if (!checkgrid()) {
							grid[cursorpos].val = temp;
						} else if (temp == 0) {
							tilesleft--;
							if (tilesleft == 0) goto win;
							printf("\x1b[0;%uHTiles Left: %u", 
								(31 - 12 - intsize(tilesleft)) / 2, 
								tilesleft
							);
						}
					
					}

			}

		if (grid[cursorpos].use == 0) {
			goto mainloopkeys;
		}

		mainloopcontinue:

			printf("\x1b[27m\x1b[%u;%uH%c", (gridpos[cursorpos] / gridx) + 2, (gridpos[cursorpos] % gridx) + 3, digittochar(grid[cursorpos].val));
		
	}

	end:

		tcsetattr(0, TCSANOW, &restore); // restore terminal state
		printf("\x1b[25h\x1b[0m\x1b[%u;0\n", gridy + 2 + 1 + 60); // TODO - 1 - 100

		return 0;

	win:
		printf("\x1b[0;12HYou Win");
		goto end;
		
}
