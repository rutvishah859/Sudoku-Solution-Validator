#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <stdbool.h>
#include "sudoku_sol_validator.h"

int board[9][9];

/* structure for passing data to threads */ 
typedef struct parameters 
{  
    int row;  
    int column;  
} parameters;

// if ith value is 1 then the region is valid, otherwise if 0 then it's invalid
int validCol[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int validRow[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int validGrid[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int main(int argc, char *argv[])
{
    // creates the sudoku board from the Lab3 puzzle.txt file
    createBoard();

    int x = 0;
    int y = 0;

    // prints the initial board
    printBoard(board);

    // solves the board
    solveSu(x, y);

    printf("\n_____________________\n\n");

    pthread_t row_tids[9];
    pthread_t col_tids[9];
    pthread_t grid_tids[9];

    // test to break program
    // board[3][5] = 6;

    // creates 9 threads to check that each row and column contains digits 1-9
    for (int i = 0; i < 9; i++) {
      parameters *p = (parameters *) malloc(sizeof(parameters));
      p->row = i;
      p->column = i;
      pthread_create(&row_tids[i], NULL, checkRow, p);
      pthread_create(&col_tids[i], NULL, checkColumn, p);
    }
    
    // creates 9 threads to check that each sub-grid contains digits 1-9
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        parameters *p = (parameters *) malloc(sizeof(parameters));
        p->row = i * 3;
        p->column = j * 3;
        pthread_create(&grid_tids[(i * 3) + j], NULL, checkGrid, p);
      }
    }

    // join all threads for rows, columns and sub-grids
    for (int i = 0; i < 9; i++) {
      pthread_join(row_tids[i], NULL);
      pthread_join(col_tids[i], NULL);
      pthread_join(grid_tids[i], NULL);
    }

    // check if board contains any 0 values since that makes it invalid
    for (int i = 0; i < 9; i++) {
      // printf("valid col is %d row is %d and grid is %d\n", validCol[i], validRow[i], validGrid[i]);
      if (validCol[i] == 0 || validRow[i] == 0 || validGrid[i] == 0) {
        printf("The sudoku board is not valid\n");
        return 0;
      }
    }
    printBoard(board); // prints the solution board

    // opens a solution.txt file if exists, otherwise creates one
    FILE *fp;
    fp = fopen("solution.txt", "w");

    // file error
    if (fp == NULL) {
      printf("Couldn't create file\n");
      return 0;
    }

    // writes each line of solution board to solution file
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        fprintf(fp, "%d ", board[i][j]);

        if (j == 2 || j == 5) {
          fprintf(fp, "| ");
        }
      }
      fprintf(fp, "\n");
      if (i == 2 || i == 5) {
        fprintf(fp, "---------------------\n");
      }
    }
    fclose(fp); // close file

    /* Success message */
    printf("\nFile created and solution saved successfully. \n");
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
  // prints the solution board with sub-grid separators
  for (int i = 0; i  < 9; i++) {
    for (int j = 0; j < 9; j++){
      printf("%d ", b[i][j]);

      if (j == 2 || j == 5) {
        printf("| ");
      }
    }
    printf("\n");
    if (i == 2 || i == 5) {
      printf("---------------------\n");
    }
  }
}

int sameCol(int x, int y, int n){
  // checks if n already exists in the same column
  for(int i = 0; i < 9; i++){
    if(board[x][i] == n){
        return 1;
    }
  }

  return 0;
}

int sameRow(int x, int y, int n){
  // checks if n already exists in same row
  for(int i = 0; i < 9; i++){
    if(board[i][y] == n){
        return 1;
    }
  }

  return 0;
}

int sameGrid(int x, int y, int n){
  // checks if n already exists in the sub-grid

  // initializes row indexes to their corresponding sub-grid
  if(x < 3){
    x = 0;
  }else if(x < 6){
    x = 3;
  }else{
    x = 6;
  }

  // initializes column indexes to their corresponding sub-grid
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
    // solves the sudoku board
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
            // if n isn't in same row, column or sub grid then place on board
            if(!sameRow(x, y, n) && !sameCol(x, y, n) && !sameGrid(x, y, n)){
                
                board[x][y] = n;

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
                    return 1;
                }
            }
            n++;
        }

        board[x][y] = 0;
        return 0;
    }
}

// check each column of the sudoku board, validate of all the values in the column are unique [1, 9]
void *checkColumn(void *param) {
  parameters p = *(parameters*)param;
  int col = p.column;
	
	int isValid = 1;
	
	int cols[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	for(int i = 0; i < 9; i++){
		if(board[i][col] > 0){
			cols[board[i][col]] += 1;
		}
	}
	
	for(int i = 0; i < 10; i++){
		if(cols[i] > 1){
			isValid = 0;
			break;
		}
	}
	validCol[col] = isValid;
  pthread_exit(0);
}

// check each row of the sudoku board, validate of all the values in the row are unique [1, 9]
void *checkRow(void *param) {
  parameters p = *(parameters*)param;
  int row = p.row;
	
	int isValid = 1;
	
	int rows[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	for(int i = 0; i < 9; i++){
		if(board[row][i] > 0){
			rows[board[row][i]] += 1;
		}
	}
	
	for(int i = 0; i < 10; i++){
		if(rows[i] > 1){
			isValid = 0;
			break;
		}
	}
	validRow[row] = isValid;
  pthread_exit(0);
}

// check each individual 3-by-3 grid, validate of all the values in the grid are unique [1, 9]
void *checkGrid(void *param) {
  parameters p = *(parameters*)param;
  int grids[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  int isValid = 1;


  int box_x = p.row / 3;
  int box_y = p.column /3;

  int box_num = box_x * 3 + box_y;

  for(int i = box_x * 3; i < box_x*3 + 3; i++){
    for(int j = box_y *3; j < box_y*3 + 3; j++){
      if (board[i][j] > 0){
        grids[board[i][j]] += 1;
      }
    }
  }

  for(int i = 0; i < 10; i++){

    if(grids[i] > 1){
      isValid = 0;
      break;
    }
  }

  validGrid[box_num] = isValid;
  pthread_exit(0);
}