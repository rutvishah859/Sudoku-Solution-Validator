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

    // test invalid board
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

//function to create a 9x9 2-D array to represent the sudoku board
void createBoard() {
  // open given arg file and read the file to extract the commands to run
	FILE *fptr;
	fptr = fopen("Lab3 puzzle.txt", "r");

  // file error
	if (fptr == NULL) {
		printf("Couldn't open file\n");
    exit(0);
	}

  // make a 9x9 2-D sudoku array from the Lab3 puzzle.txt file
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


//function to iterate through each element of the 2-D sudoku array and print it out with some formatting 
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

//function to check if a given number 'n' exists in the column in which it was placed
int sameCol(int x, int y, int n){
  //iterate through the column specified by 'x'
  //if 'n' exists in the same column, return 1
  for(int i = 0; i < 9; i++){
    if(board[x][i] == n){
        return 1;
    }
  }

  //The number 'n' was not found anywhere else in the column (i.e. it is valid to put this 'n' in this column)
  return 0;
}

//function to check if a given number 'n' exists in the row in which it was placed
int sameRow(int x, int y, int n){
  //iterate through the row specified by 'y'
  //if 'n' exists in the same row, return 1
  for(int i = 0; i < 9; i++){
    if(board[i][y] == n){
        return 1;
    }
  }

  //The number 'n' was not found anywhere else in the row (i.e. it is valid to put this 'n' in this row)
  return 0;
}

//function to check if a given number 'n' exists in the 3x3 sub-grid in which it was placed
int sameGrid(int x, int y, int n){
  // checks if n already exists in the sub-grid

  // initializes row index to its corresponding sub-grid
  if(x < 3){
    x = 0;
  }else if(x < 6){
    x = 3;
  }else{
    x = 6;
  }

  // initializes column index to its corresponding sub-grid
  if(y < 3){
    y = 0;
  }else if(y < 6){
    y = 3;
  }else{
    y = 6;
  }

  //iterate through the elements in the sub-grid specified by the indices chosen above
  //return 1 if 'n' exists in that sub-grid
  for(int i = x; i < x+3; i++){
    for(int j = y; j < y+3; j++){

      if(board[i][j] == n){
        return 1;
      }

    }
  }

  //The number 'n' was not found anywhere else in the sub-grid (i.e. it is valid to put this 'n' in this sub-grid)
  return 0;
}

//function to solve the sudoku board recursively using a backtracking algorithm
int solveSu(int x, int y){
    int n = 1; //the number to be placed into the sudoku board to attempt to solve it

    //temporary row and column variables
    int tx = 0;
    int ty = 0;

    
    if(board[x][y] != 0){ //if the current x-y position in the 2-D sudoku array is not a valid position to place a "guess"
        
        //to wrap the row/column variables around
        if(x == 8 && y == 8){
            return 1;
        }


        if(x < 8){
            x++; //increment column number if it is less than 8
        }else{
            if(y < 8){
                x = 0; //go to the beginning of the row (i.e. 0'th column)
                y++;  //increment the row number if it is less than 8
            }
        }

        if(solveSu(x, y)){ //recursively call the solver function with the new row-column indices
            return 1; //return 1 (true) if the solution can be reached with previous function call
        }else{
            return 0; //return 0 (false) if solution cannot be reached with previous function call
        }
    }

    if(board[x][y] == 0){ //if the current x-y position in the 2-D sudoku array is a valid position to place a "guess"
        
        while(n < 10){ //loop for values of 'n' between 1-9 (inclusive)
            // if n isn't in same row, column or sub grid 
            if(!sameRow(x, y, n) && !sameCol(x, y, n) && !sameGrid(x, y, n)){
                
                //then place it on the board at row-x, column-y
                board[x][y] = n;

                //same wrapping as above
                if(x == 8 && y == 8){
                    return 1;
                }

                //initialize a the temporary row-column variables and continue
                //these temporary positions are needed because it is not yet guaranteed that the 'n' placed
                //on the board at the previous step is correct to the solution of the board.
                //Using temporary position allows for backtracking without explicitly saving the original indices
                //in a data-structure (such as a stack).
                if(x < 8){
                    tx = x+1;
                }else{
                    if(y < 8){
                        tx = 0;
                        ty = y + 1;
                    }
                }

                //recursively call the solver function; this time with the temporary indices
                //if the solver returns true, return 1 from this if-else block
                if(solveSu(tx, ty)){
                    return 1;
                }
            }
            n++; //increment n
          
        }

        //this area is only reached when backtracking is needed (i.e. if the numbers placed on the board above do not
        //lead to a solution).
        //In this can, replace the position at x-y with a 0 again (to reset it)
        board[x][y] = 0;

        //return 0 (false) if the solution is not being reached (this is used by the recursive function calls above) 
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