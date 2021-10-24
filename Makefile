main.o:
	gcc main.c -O3 -s -Wall -Wextra -o sudoku -lpthread
	./sudoku

debug:
	clang main.c -O3 -s -Wall -Wextra -o sudoku -g -lpthread
	-valgrind --dsymutil=yes --track-origins=yes --leak-check=full ./sudoku 

compile:
	gcc main.c -O3 -s -Wall -Wextra -o sudoku -lpthread
