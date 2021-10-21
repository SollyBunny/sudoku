main.o:
	gcc main.c -O3 -s -Wall -Wextra -o sudoku
	./sudoku

debug:
	clang main.c -O3 -s -Wall -Wextra -o sudoku -g
	-valgrind --dsymutil=yes --track-origins=yes --leak-check=full ./sudoku

compile:
	gcc main.c -O3 -s -Wall -Wextra -o sudoku
