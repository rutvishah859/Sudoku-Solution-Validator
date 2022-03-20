#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 

/* structure for passing data to threads */ 
typedef struct  
{  
    int row;  
    int column;  
} parameters;

int board[9][9];

// if ith value is 1 then the region is valid, otherwise if 0 then it's invalid
int validCol[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int validRow[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int validGrid[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// check each column of the sudoku board, validate of all the values in the column are unique [1, 9]
void *checkColumn(void *param) {
  
}

// check each row of the sudoku board, validate of all the values in the row are unique [1, 9]
void *checkRow(void *param) {

}

// check each individual 3-by-3 grid, validate of all the values in the grid are unique [1, 9]
void checkGrid(void *param) {

}

// prints formatted sukoku board
void printBoard () {
  for (int i = 0; i  < 9; i++) {
    for (int j = 0; j < 9; j++){
      printf("%d ", board[i][j]);

      if (j == 2 || j == 5) {
        printf("| ");
      }
      if (j == 8) {
        printf("\n");
      }
    }
    if (i == 2 || i == 5) {
      printf("------------------\n");
    }
  }
}

int main(void) { 
  // how to make param struct 
  // parameters *data = (parameters *) malloc(sizeof(parameters));
  // data->row = 1;
  // data->column = 1; 

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

  // get puzzle solver inputs
  // int puzzleRow; 
  // int puzzleCol;
  printBoard();
} 