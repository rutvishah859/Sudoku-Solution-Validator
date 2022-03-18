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

// if ith value is 1 then the region is valid, otherwise if 0 then it's invalid
int validCol[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int validRow[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int validGrid[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// check each column of the sudoku board, validate of all the values in the column are unique [1, 9]
void *checkColumn(void *param) {
  parameters *data = (parameters *) malloc(sizeof(parameters));
  data->row = 1;
  data->column = 1;  
}

// check each row of the sudoku board, validate of all the values in the row are unique [1, 9]
void *checkRow(void *param) {

}

// check each individual 3-by-3 grid, validate of all the values in the grid are unique [1, 9]
void checkGrid(void *param) {

}

int main(void) { 
  
} 