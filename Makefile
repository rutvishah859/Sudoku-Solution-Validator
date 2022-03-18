all: sudoku_sol_validator.c
	gcc -Wall -Wextra -std=c99 -pthread sudoku_sol_validator.c -o sudoku_sol_validator
	./sudoku_sol_validator