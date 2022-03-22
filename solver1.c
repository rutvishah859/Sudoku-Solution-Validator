#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <stdbool.h>
#include "solver1.h"

int board[9][9];

int main(int argc, char *argv[])
{
    createBoard();

    int x = 0;
    int y = 0;

    printBoard(board);
    solveSu(x, y);
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

int sameCol(int x, int y, int n){

    for(int i = 0; i < 9; i++){

        if(board[x][i] == n){
            return 1;
        }
    }

    return 0;
}

int sameRow(int x, int y, int n){

    for(int i = 0; i < 9; i++){

        if(board[i][y] == n){
            return 1;
        }
    }

    return 0;
}

int sameGrid(int x, int y, int n){

    if(x < 3){
        x = 0;
    }else if(x < 6){
        x = 3;
    }else{
        x = 6;
    }

    if(y < 3){
        y = 0;
    }else if(y < 6){
        y = 3;
    }else{
        y = 6;
    }

    for(int i = x; i < x+3; i++){
        for(int j = y; j < y+3; j++){

            if(board[i][j] == n){
                return 1;
            }

        }
    }

    return 0;
}

int solveSu(int x, int y){
    int n = 1;

    int tx = 0;
    int ty = 0;

    if(board[x][y] != 0){
        
        if(x == 8 && y == 8){
            return 1;
        }

        if(x < 8){
            x++;
        }else{
            if(y < 8){
                x = 0;
                y++;
            }
        }

        if(solveSu(x, y)){
            return 1;
        }else{
            return 0;
        }
    }

    if(board[x][y] == 0){
        while(n < 10){
            if(!sameRow(x, y, n) && !sameCol(x, y, n) && !sameGrid(x, y, n)){
                
                board[x][y] = n;

                // printf("%d %d\n", p.column, p.row);
                // printf("\n_____________________\n");
                // printBoard(board);

                if(x == 8 && y == 8){
                    
                    return 1;
                }

                if(x < 8){
                    tx = x+1;
                }else{
                    if(y < 8){
                        tx = 0;
                        ty = y + 1;
                    }
                }

                if(solveSu(tx, ty)){
                    // printf("looping\n");
                    return 1;
                }
            }
            n++;
        }

        board[x][y] = 0;
        return 0;
    }
}