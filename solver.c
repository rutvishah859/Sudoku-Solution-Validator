#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <stdbool.h>
#include "solver.h"

int board[9][9];

int main(int argc, char *argv[])
{
    createBoard();

    parameters p;

    p.row = 0;
    p.column = 0;

    printBoard(board);
    solveSu(p);
    printf("\n_____________________\n");
    printBoard(board);
    return 0;
}

void createBoard() {
  // open given arg file and read the file to extract the commands to run
	FILE *fptr;
	fptr = fopen("Lab3 puzzle.txt", "r");

  // file error
	if (fptr == NULL) {
		printf("Couldn't open file\n");
    exit(0);
	}

  // make a 9x9 sudoku array from the Lab3 puzzle.txt file
  int i = 0;
  int j = 0;
  while (fscanf(fptr, "%d", &board[i][j]) == 1){
    if (j == 8) {
      j = 0;
      i++;
      continue;
    }
    j++;
  }
}

void printBoard(int b[9][9]) {
  for (int i = 0; i  < 9; i++) {
    for (int j = 0; j < 9; j++){
      printf("%d ", b[i][j]);

      if (j == 2 || j == 5) {
        printf("| ");
      }
      if (j == 8) {
        printf("\n");
      }
    }
    if (i == 2 || i == 5) {
      printf("---------------------\n");
    }
  }
}

int sameCol(parameters p, int n){

    for(int i = 0; i < 9; i++){

        if(board[i][p.column] == n){
            return 1;
        }
    }

    return 0;
}

int sameRow(parameters p, int n){

    for(int i = 0; i < 9; i++){

        if(board[p.row][i] == n){
            return 1;
        }
    }

    return 0;
}

int sameGrid(parameters p, int n){

    int bx = p.row/3;
    int by = p.column/3;

    for(int i = bx; i < bx+3; i++){
        for(int j = by; j < by+3; j++){

            if(board[i][j] == n){
                return 1;
            }

        }
    }

    return 0;
}

int solveSu(parameters p){
    int n = 1;

    // int x = p.row;
    // int y = p.column;

    parameters tp;

    tp.row = 0;
    tp.column = 0;

    if(board[p.column][p.row] != 0){
        
        if(p.row == 8 && p.column == 8){
            return 1;
        }

        if(p.column < 8){
            p.column++;
        }else{
            if(p.row < 8){
                p.column = 0;
                p.row++;
            }
        }

        if(solveSu(p)){
            return 1;
        }else{
            return 0;
        }
    }

    if(board[p.column][p.row] == 0){
        while(n < 10){
            if(!sameRow(p, n) && !sameCol(p, n) && !sameGrid(p, n)){
                
                board[p.column][p.row] = n;

                // printf("%d %d\n", p.column, p.row);
                // printf("\n_____________________\n");
                // printBoard(board);

                if(p.row == 8 && p.column == 8){
                    
                    return 1;
                }

                if(p.column < 8){
                    tp.column = p.column+1;
                }else{
                    if(p.row < 8){
                        tp.column = 0;
                        tp.row = p.row + 1;
                    }
                }

                if(solveSu(tp)){
                    printf("looping\n");
                    return 1;
                }
            }
            n++;
        }

        board[p.column][p.row] = 0;
        return 0;
    }
}