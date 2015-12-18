#pragma once
#include <iostream>
#include <boost/timer.hpp>
#include <set>
#include <queue>
#include <fstream>
#include <string>
#define SIZE 9
#define DEL 3

/**
	i => lines : 0 - 8
	j => columns : 0 - 8
**/

struct Case
{
	int i;
	int j;
};

class SudokuSolver
{
private:
	int grid[SIZE][SIZE];

	std::queue<Case> file;


	//check if area is OK (no doublon and 1<=x<=9) i,j input
    bool checkArea(int _x, int _y);

	//return number of zero in an area i,j input
    int countAreaZero(int _x, int _y);

public:

    SudokuSolver();

    void importGrid(int _grid[SIZE][SIZE]);

	
    void print();


	//check if line n is correct (no doublon 1<=x<=9)
    bool checkLine(int n);

	//same for column
    bool checkColumn(int n);

	//check sqaure from numero of square
    bool checkSquare(int numero);

	//check if grid contain void case (= 0)
    bool hasZero();

	//initialize queue push each void case
    void initQueue();

	//return the next void case (start up-left)
    int nextVoidCase();

	//return number of line containing most number of zero
    int bestLine();

	//return column number containing most nuymber of zero
    int bestColumn();

	//return numero of square containing most number of zero
    int bestSquare();

	//return vector containing every possibilies of a case, input i, j
    std::vector<int> possibility(int i, int j);


    bool isPresentLine(int v, int l);
    bool isPresentCol(int v, int c);
    bool isPresentBloc(int v, int x, int y);

    bool isValid(int position);

    bool optIsValid(int position);
    bool megaOptIsValid(int position);

    void import9x9FromFile();
};



