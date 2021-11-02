#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

// Key codes
#define BREAK 3

#define KEYW 119
#define KEYA 97
#define KEYS 115
#define KEYD 100

#define KEY0 48

#define GRIDSIZE (9 * 9) // shoudnt be changed
#define DIFFICULTY 2

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

// util function turn a digit to a char
#define digittochar(a) (a==0) ? (' ') : (a+48)

// util function to get base 10 intiger "size"
unsigned int intsize(unsigned int a) {
	if      (a < 10       ) return 1;
	else if (a < 100      ) return 2;
	else                    return 3;
}

// checks if the current grid is valid
int gridcheck() {

	// These lists store the tiles in the column/row/block to see if there is a repeat
	unsigned int row[9]; // row (9x1)
	unsigned int col[9]; // column (1x9)
	unsigned int blk[9]; // block (3x3)
	
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

			for (unsigned int _ = 0; _ < m; ++_) { // only have to compare upto whatve been set, instead of comparing a bunch of 0's
				if ((row[_] == row[m]) && (row[_] != 0)) return 0;
				if ((col[_] == col[m]) && (col[_] != 0)) return 0;
				if ((blk[_] == blk[m]) && (blk[_] != 0)) return 0;
			}
		}

	}

	return 1;
	
}

void timerfunc(int * clock) {

	// printf("%u", *argsthread);

	// unsigned int time = 0;

	while (1) {
		if ((*clock) != -1) {
			printf("\x1b[7m\x1b[%u;%uHTime: %02u:%02u", 
				gridy + 2,
				(31 - 11 - intsize(GRIDSIZE)) / 2, 
				(*clock) / 60, 
				(*clock) % 60
			);
			fflush(stdout);
			(*clock)++;
		}
		usleep(1000000);
	}
}

int main(void) {

	int* clock = malloc(sizeof(int));

	pthread_t timerthread;
	// start the timer thread
	pthread_create(
		&timerthread, 
		NULL, 
		(void *)&timerfunc, 
		clock
	);

	srand(time(0));

	start:
	*clock = -1;

	for (unsigned int i = 0; i < GRIDSIZE; ++i) {
		grid[i].val = (
			i + 1 + // inital 1-9 pattern
			(i / 9 * 3) + // offset 3 per row
			(i / (9 * 3)) // offset 9 per large row
		) % 9 + 1;
		grid[i].use = 0; // need to initialize (due to restarts)
	}

	// Genrate grid
	unsigned int temp;
	unsigned int r1;
	unsigned int r2;

	for (unsigned int i = 0; i < 20; ++i) { gridgenloopstart:
		
		for (unsigned int j = 0; j < GRIDSIZE; ++j) { // move grid into tempgrid as a backup
			tempgrid[j] = grid[j].val;
		}

		// choose a random operation
		if (rand() % 2 < 1) { // swap columns
			r1 = (rand() % 9) * 9;
			r2 = (rand() % 9) * 9;
			for (unsigned int m = 0; m < 9; ++m) {
				temp = grid[r1 + m].val;
				grid[r1 + m].val = grid[r2 + m].val;
				grid[r2 + m].val = temp; 
			}
		} else { // swap rows
			r1 = (rand() % 9);
			r2 = (rand() % 9);
			for (unsigned int m = 0; m < 9; ++m) {
				temp = grid[r1 + (m * 9)].val;
				grid[r1 + (m * 9)].val = grid[r2 + (m * 9)].val;
				grid[r2 + (m * 9)].val = temp; 
			}
		}

		if (!gridcheck()) { // if it fails the gridcheckm, swap the tempgrid back into grid and try again
			for (unsigned int j = 0; j < GRIDSIZE; ++j) {
				grid[j].val = tempgrid[j];
			}
			goto gridgenloopstart;
		}
		
	}

	tilesleft = 0;
	for (unsigned int i = 0; i < GRIDSIZE / DIFFICULTY; ++i) { generateusestart:
	//for (unsigned int i = 0; i < 1; ++i) { generateusestart:
		r1 = rand() % GRIDSIZE;
		if (grid[r1].val == 0) goto generateusestart;
		grid[r1].val = 0;
		grid[r1].use = 1;
		tilesleft++;
	}

	// set terminal settins (dont echo characters / queue stdin / ignore ctrl-c )
	struct termios term, restore;
	tcgetattr(0, &term);
	tcgetattr(0, &restore); // backup the original terminal state to restore later
	term.c_lflag &= ~(ICANON|ECHO|ISIG);
	tcsetattr(0, TCSANOW, &term);

	// print the board
	/*
		The grid is stored in `gridtxt` which is just a charstring
		the charstring has the whole board but instead of numbers there are `A` characters
		each time an `A` is found it will be replaced by the value of the pointer of the grid and then iterated
		this means i dont have to have any weird psuedo loops anywhere
	*/
	printf("\x1b[2J\x1b[48;5;237m\x1b[?25l\x1b[H\x1b[1m\x1b[7m"); // clear, fg: grey, bg: white, hide cursor, bold, reverse

	char * i;
	struct Tile * m;

	// loop through the grid
	m = grid;
	for (i = gridtxt; *i; ++i) {
		if (*i == 'A') {
			putchar(digittochar((*m).val)); // print the number
			++m;
		} else if (*i == '\n') { // newline has padding
			putchar(' ');
			putchar(' ');
			putchar('\n');
			putchar(' ');
			putchar(' ');
		} else {
			putchar(*i);
		}
	}

	// prints the current amount of empty tiles left to fill (top center)
	printf("\x1b[22m\x1b[0;%uHTiles Left: %0*u", // unbold
		(31 - 12 - intsize(GRIDSIZE)) / 2, 
		intsize(GRIDSIZE), tilesleft
	);
	
	//goto win; // DEBUG (print & exit)

	// set initial cursor position
	cursorpos = GRIDSIZE / 2; // in the middle of the screen
	while (grid[cursorpos].use == 0) { // if the middle is taken up by a tile, keep iterating it by 1 untill an empty one is found
		cursorpos++;
		if (cursorpos > GRIDSIZE) cursorpos = 0; // if loops of edge of grid reset to 0
	}

	*clock = 0;

	char c;

	goto mainloopcontinue; // skip first loop (to draw cursor)

	while ((c = getchar()) != BREAK) {

		// clear previous cursor
			printf("\x1b[7m\x1b[%u;%uH%c", (gridpos[cursorpos] / gridx) + 2, (gridpos[cursorpos] % gridx) + 3, digittochar(grid[cursorpos].val));


		mainloopkeys: switch (c) {

			case KEYW:
				cursorpos -= 9;
				if (cursorpos > GRIDSIZE) { // loop top
					cursorpos += GRIDSIZE;
				}
				break;
				
			case KEYS:
				cursorpos += 9;
				if (cursorpos > GRIDSIZE) { // loop bottom
					cursorpos -= GRIDSIZE;
				}
				break;
				
			case KEYA:
				if (cursorpos % 9 == 0) { // loop left
					cursorpos += 9;
				}
				cursorpos--;
				break;
				
			case KEYD:
				if (cursorpos % 9 == 9 - 1) { // loop right
					cursorpos -= 9;
				}
				cursorpos++;
				break;

			case KEY0:
				if (grid[cursorpos].val == 0) continue; // if the tile is alredy blank, cotninue to next loop
				grid[cursorpos].val = 0; // otherwise set it to 0
				tilesleft++; // a tile was just freed so add 1 to tilesleft
				// prints the current amount of empty tiles left to fill (top center)
				printf("\x1b[0;%uHTiles Left: %0*u", 
					(31 - 12 - intsize(GRIDSIZE)) / 2, 
					intsize(GRIDSIZE), tilesleft
				);
				break;

			default:

				if (c > KEY0 && c <= KEY0 + 9) { // key: 0-9 inclusive

					temp = grid[cursorpos].val;
					grid[cursorpos].val = c - KEY0;	// set the cursorpos to the key pressed
					if (!gridcheck()) { // if the attempted value is invalid in the grid, undo the action
						grid[cursorpos].val = temp;
					} else if (temp == 0) {
						tilesleft--;
						if (tilesleft == 0) goto win; // if all tiles are filled (and are valid), win the game
						// prints the current amount of empty tiles left to fill (top center)
						printf("\x1b[0;%uHTiles Left: %0*u", 
							(31 - 12 - intsize(GRIDSIZE)) / 2, 
							intsize(GRIDSIZE), tilesleft
						);
					}

					break;
				
				}

				continue;  // skips any rendering if invalid key

		}

		if (grid[cursorpos].use == 0) { // if the tile which u moved too 
			goto mainloopkeys;
		}

		mainloopcontinue:
			printf("\x1b[27m\x1b[%u;%uH%c", (gridpos[cursorpos] / gridx) + 2, (gridpos[cursorpos] % gridx) + 3, digittochar(grid[cursorpos].val));
		
	}

	/* // previous printfs, combined into 1 here
		printf("\x1b[7m\x1b[%u;%uH%c", (gridpos[cursorpos] / gridx) + 2, (gridpos[cursorpos] % gridx) + 3, digittochar(grid[cursorpos].val));
		printf("\x1b[7m\x1b[0;8H ~~ You Win ~~  "); // padding at the start to clear any reminants of "Tiles Left: x"
		tcsetattr(0, TCSANOW, &restore); // restore terminal state
		printf("\x1b[25h\x1b[0m\x1b[%u;0H\n", gridy + 2);
	*/

	end:

		/* 
			End:
			1. reset all modes and goto the end of the grid (show cursor, reset color/bold, goto gridy + 2)
		*/

		*clock = -1;
		printf("\x1b[?25h\x1b[0m\x1b[%u;0H\nRestart? (y/n)", gridy + 2);
		do { // wait for valid input, if y reset, otherwise quit
			c = getchar();
			//putchar(c);
		} while (c != 121 && c != 110 && c != BREAK); // 121 = y, 110 = n
		if (c == 121)
			goto start;

		//printf("", );
		tcsetattr(0, TCSANOW, &restore); // restore terminal state
		return 0;

	win:

		/*
			Win:
			1. clear the cursor, to give a clean board on win
			2. print the "you win" text in the same place as the "Tiles left" text was, use padding on either side to hide any reminants
		*/
		printf("\x1b[7m\x1b[%u;%uH%c\x1b[0;8H ~~ You Win ~~", (gridpos[cursorpos] / gridx) + 2, (gridpos[cursorpos] % gridx) + 3, digittochar(grid[cursorpos].val));

		goto end;
		
}
