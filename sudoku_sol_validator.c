#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <stdbool.h>

/* structure for passing data to threads */ 
typedef struct parameters 
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
	
  parameters p = *(parameters*)param;

  int col = p.column;
  
  // int col = *(int*)param;
	
	int isValid = 1;
	
	int cols[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	for(int i = 0; i < 9; i++){
		if(cols[board[i][col]] > 0){
			cols[board[i][col]] += 1;
		}
	}
	
	for(int i = 0; i < 9; i++){
		if(cols[i] > 1){
			isValid = 0;
			break;
		}
	}
	validCol[col] = isValid;
}

// check each row of the sudoku board, validate of all the values in the row are unique [1, 9]
void *checkRow(void *param) {

  parameters p = *(parameters*)param;

  int row = p.row;

  //int row = *(int*)param;
	
	int isValid = 1;
	
	int rows[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	for(int i = 0; i < 9; i++){
		if(rows[board[row][i]] > 0){
			rows[board[row][i]] += 1;
		}
	}
	
	for(int i = 0; i < 9; i++){
		if(rows[i] > 1){
			isValid = 0;
			break;
		}
	}
	validRow[row] = isValid;
}

// check each individual 3-by-3 grid, validate of all the values in the grid are unique [1, 9]
void checkGrid(void *param) {

}

int find_empty(int b[9][9], int* x, int* y){

  for (int i = 0; i < 9; i++){
    for(int j = 0; j < 9; j++){
      if(b[i][j] == 0){
        *x = i;
        *y = j;

        return 1;
      }
    }
  }
}

// prints formatted sudoku board
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
      printf("------------------\n");
    }
  }
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

int main(void) { 
  // how to make param struct 
  // parameters *data = (parameters *) malloc(sizeof(parameters));
  // data->row = 1;
  // data->column = 1; 

  createBoard();
  
  printBoard(board);

  

  int x, y;

  find_empty(board, &x, &y);

  parameters p;

  p.row = x;

  p.column = y;

  printf("(%d, %d)\n", p.row, p.column);
  
  pthread_t colVal, rowVal;
    
  pthread_create(&colVal, NULL, &checkColumn, &p);
  pthread_join(colVal, NULL);

  pthread_create(&rowVal, NULL, &checkRow, &p);
  pthread_join(rowVal, NULL);
    
  printf("%d %d\n", validCol[p.column], validCol[p.column+1]);
  printf("%d %d\n", validRow[p.row], validCol[p.row+1]);
}
 
