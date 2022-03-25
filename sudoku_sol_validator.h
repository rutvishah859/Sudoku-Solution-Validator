#pragma once



void createBoard();

void printBoard(int[9][9]);

int sameCol(int, int, int);
int sameRow(int, int, int);
int sameGrid(int, int, int);

int solveSu(int, int);

void *checkColumn(void *param); 
void *checkRow(void *param);
void *checkGrid(void *param); 