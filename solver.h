#pragma once

typedef struct parameters 
{  
    int row;  
    int column;  
} parameters;

void createBoard();

void printBoard(int[9][9]);

int sameCol(parameters, int);
int sameRow(parameters, int);
int sameGrid(parameters, int);

int solveSu(parameters);