#include "sudokuSolver.h"

SudokuSolver::SudokuSolver(){
    for(int i = 0 ; i< SIZE; ++i)
        for(int j = 0; j < SIZE; ++j)
            grid[i][j] = 0;
}



void SudokuSolver::initQueue()
{
    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            if(grid[i][j] == 0)
            {
                Case c;
                c.i = i;
                c.j = j;
                file.push(c);
            }
        }
    }
}

int SudokuSolver::nextVoidCase(){
    Case c = file.front();
    file.pop();
    return c.i * SIZE + c.j;
}

std::vector<int> SudokuSolver::possibility(int i, int j)
{
    bool res[10];
    for(int a = 0; a < 10; ++a)
        res[a] = true;

    for(int a = 0; a < SIZE; ++a)
    {
        res[grid[i][a]] = false;
        res[grid[a][j]] = false;
    }
    int startLine = (i / B_HEIGHT)*3;
    int startCol = (j / B_WIDTH)*3;
    for(int a = 0; a < B_HEIGHT; ++a)
        for(int b = 0; b < B_WIDTH; ++b)
            res[grid[startLine+a][startCol+b]] = false;
    std::vector<int> v;
    for(int a = 1; a < 10; ++a)
        if(res[a])
            v.push_back(a);
    return v;
}


bool SudokuSolver::isPresentLine(int v, int l)
{
    for(int j = 0; j < SIZE; ++j)
    {
        if(grid[l][j] == v)
            return true;
    }
    return false;
}
bool SudokuSolver::isPresentCol(int v, int c)
{
    for(int i = 0; i < SIZE; ++i)
    {
        if(grid[i][c] == v)
            return true;
    }
    return false;
}
bool SudokuSolver::isPresentBloc(int v, int x, int y)
{
    int x_bloc = B_HEIGHT*(x/B_HEIGHT), y_bloc = B_WIDTH*(y/B_WIDTH);
    for(int i = 0; i < SIZE / B_HEIGHT; ++i)
        for(int j = 0; j < SIZE / B_WIDTH; ++j)
            if(grid[x_bloc+i][y_bloc+j] == v)
                return true;
    return false;
}

bool SudokuSolver::isValid(int position)
{
    if(position == SIZE*SIZE)
        return true;

    int i = position/SIZE, j = position%SIZE;

    if(grid[i][j] != 0)
        return isValid(position+1);

    for(int k=1; k <= SIZE; k++)
    {
        if(!isPresentLine(k,i) && !isPresentCol(k,j) && !isPresentBloc(k,i,j))
        {
            grid[i][j] = k;

            if(isValid(position+1))
                return true;
        }
    }
    grid[i][j] = 0;
    return false;
}

void SudokuSolver::foundNumbers(std::vector<std::vector<int>>& _foundPositions)
{
    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            if(grid[i][j] != 0)
            {
                _foundPositions.push_back({i, j});
            }
        }
    }
}


bool contains(std::vector<std::vector<int>>& _stockage, int _x, int _y)
{
    for(auto i : _stockage)
    {
        if(i.at(0) == _x && i.at(1) == _y)
        {
            return true;
        }
    }
    return false;
}

void SudokuSolver::hideExceptOne()
{
    srand (time(NULL));
    std::vector<std::vector<int>> positions;
    foundNumbers(positions);
    isValid(0);
    int x,y;
    do
    {
        x = rand() % SIZE;
        y = rand() % SIZE;

    }while(contains(positions, x, y));
    positions.push_back({x, y});
    print();
    std::cout << positions.size() << std::endl;

    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            if(!contains(positions, i, j))
            {
                grid[i][j] = 0;
            }
        }
    }
    notify();
}

bool SudokuSolver::isFilled() const
{
    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            if(grid[i][j] == 0)
                return false;
        }
    }
    return true;
}

bool SudokuSolver::solve()
{
    bool ret = isValid(0);
    notify();
    return ret;
}

//**********************************************************************************************************************
bool SudokuSolver::optIsValid(int position)
{
    if(position == SIZE*SIZE)
        return true;

    int i = position/SIZE, j = position%SIZE;

    if(grid[i][j] != 0)
        return isValid(position+1);

    std::vector<int> v = possibility(i,j);
    for(auto k : v)
    {
        grid[i][j] = k;
        if(isValid(position+1))
            return true;
    }
    grid[i][j] = 0;

    return false;
}

bool SudokuSolver::megaOptIsValid(int position)
{
    if(position == SIZE*SIZE)
        return true;

    int i = position/SIZE, j = position%SIZE;

    if(grid[i][j] != 0)
        return isValid(nextVoidCase());

    std::vector<int> v = possibility(i,j);
    for(auto k : v)
    {
        grid[i][j] = k;
        if(isValid(nextVoidCase()))
            return true;
    }
    grid[i][j] = 0;
    Case c;
    c.i = i;
    c.j = j;
    file.push(c);
    return false;
}

void SudokuSolver::importGridFromFile(const std::string& _filename)
{
    std::string line;
    std::ifstream myfile (_filename);
    if (myfile.is_open())
    {
        int l = 0;
        for(int i = 0; i < SIZE; ++i)
        {
            for(int j = 0; j <= SIZE; ++j)
            {
                if(j == SIZE)
                    myfile.get();
                else
                    grid[j][i] = myfile.get()-48;
            }

        }
        myfile.close();
    }
    print();
    notify();
}

void SudokuSolver::importGrid(int _grid[SIZE][SIZE])
{
    for(int i = 0 ; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            grid[i][j] = _grid[i][j];
        }
    }
}

void SudokuSolver::print(){
    std::cout << "======================== start ======================" << std::endl;
    for(int i = 0 ; i < SIZE; ++i)
    {
        if(i % B_HEIGHT == 0)
            std::cout << std::endl;
        for(int j = 0; j < SIZE; ++j)
        {
            if(j % B_WIDTH == 0)
                std::cout << " ";
            std::cout << grid[j][i];
        }
        std::cout << std::endl;
    }
    std::cout << "======================== end =======================" << std::endl;
}

int SudokuSolver::bestLine()
{
    int line = 0, maxZero = 0;
    for(int i = 0; i < SIZE; ++i)
    {
        int nb = 0;
        for(int j = 0; j < SIZE; ++j)
        {
            if(grid[i][j] == 0)
                ++nb;
        }
        if(nb > maxZero)
        {
            maxZero = nb;
            line = i;
        }
    }
    return line;
}
int SudokuSolver::bestColumn()
{
    int col = 0, maxZero = 0;
    for(int j = 0; j < SIZE; ++j)
    {
        int nb = 0;
        for(int i = 0; i < SIZE; ++i)
        {
            if(grid[i][j] == 0)
                ++nb;
        }
        if(nb > maxZero)
        {
            maxZero = nb;
            col = j;
        }
    }
    return col;
}
int SudokuSolver::bestSquare()
{
    int res[9];
    res[0] =  countAreaZero(0,0);
    res[1] =  countAreaZero(0,3);
    res[2] =  countAreaZero(0,6);
    res[3] =  countAreaZero(3,0);
    res[4] =  countAreaZero(3,3);
    res[5] =  countAreaZero(3,6);
    res[6] =  countAreaZero(6,0);
    res[7] =  countAreaZero(6,3);
    res[8] =  countAreaZero(6,6);
    int square = 0, max = 0;
    for(int i = 0 ; i < 9; ++i)
    {
        if(max < res[i])
        {
            max = res[i];
            square = i;
        }
    }
    return square;
}



bool SudokuSolver::checkArea(int _x, int _y)
{
    std::set<int> tmp;
    for(int i = 0; i < B_HEIGHT; ++i)
    {
        for(int j = 0; j < B_WIDTH; ++j)
        {
            if(!tmp.insert(grid[_x+i][_y+j]).second)
                return false;
            //if(grid[x+i][y+j] < 1 || grid[x+i][y+j] > 9)
            //	return false;
        }
    }
    return true;
}

int SudokuSolver::countAreaZero(int _x, int _y)
{
    int nb = 0;
    for(int i = 0; i < B_HEIGHT; ++i)
    {
        for(int j = 0; j < B_WIDTH; ++j)
        {
            if(grid[_x+i][_y+j] == 0)
                nb++;
        }
    }
    return nb;
}



bool SudokuSolver::checkLine(int n)
{
    std::set<int> tmp;
    for(int j = 0; j < SIZE; ++j){
        if(!tmp.insert(grid[n][j]).second)
            return false;
        //if(grid[n][j] < 1 || grid[n][j] > 9)
        //	return false;
    }
    return true;
}
bool SudokuSolver::checkColumn(int n)
{
    std::set<int> tmp;
    for(int i = 0; i < SIZE; ++i){
        if(!tmp.insert(grid[i][n]).second)
            return false;
        //if(grid[i][n] < 1 || grid[i][n] > 9)
        //	return false;
    }
    return true;
}

bool SudokuSolver::checkSquare(int numero)
{
    switch(numero)
    {
        case 0:
            return checkArea(0,0);
        case 1:
            return checkArea(0,3);
        case 2:
            return checkArea(0,6);
        case 3:
            return checkArea(3,0);
        case 4:
            return checkArea(3,3);
        case 5:
            return checkArea(3,6);
        case 6:
            return checkArea(6,0);
        case 7:
            return checkArea(6,3);
        case 8:
            return checkArea(6,6);
    }
}

bool SudokuSolver::hasZero()
{
    for(int i = 0 ; i < SIZE; ++i)
        for(int j = 0 ; j < SIZE; ++j)
            if(grid[i][j] == 0)
                return true;
    return false;
}

