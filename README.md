# Sudoku-Solution-Validator
A multithreaded application that determines whether the solution to a Sudoku pozzle is valid.

A Sudoku puzzle uses a 9 × 9 grid in which each column and row, as well as each of the nine 3 × 3 sub grids, must contain all of the digits 1-9.

## How it Works
The Sudoku puzzle solver will read in a file called puzzle.txt which contains the puzzle to solve, in which each row of the file contains a row of the puzzle with a space to delimit each of the numbers. Any entry of zero is a value that will be solved by the puzzle solver.

## How to Run
To run the sudoku_sol_validator.c run `make` in the terminal. Upon doing so a file called `solution.txt` will be created containing the solution to the provided Sudoku puzzle in the same format as the input file.
