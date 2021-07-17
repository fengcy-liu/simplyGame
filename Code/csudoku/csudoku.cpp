#include <iostream>
#include <stdlib.h>
#include <vector>
#include <random>
#include <time.h>
#include "common_type.h"

using namespace std;

#define ROW_NUM         (9)
#define COL_NUM         (9)
#define NUM_OF_GAMENUM  (ROW_NUM)
#define INVALID_MISSCNT (-1)

class sudoku
{
public:
    u8_t arr[ROW_NUM][COL_NUM];
    s8_t missCnt[ROW_NUM][COL_NUM];
    u16_t bit[ROW_NUM][COL_NUM];
    sudoku(void);
    ~sudoku(void) {};
    void generateTable(void);
    void clearTable(void);
    u8_t getRandNum(int row, int col);
    void show(void);
private:
    void setTableRowNumSts(int row, int num);
    void setTableColNumSts(int col, int num);
    void setTableSudoku3x3NumSts(int row, int col, int num);
    bool findMinCntPos(int *posrow, int *poscol);
};

void sudoku::generateTable(void)
{
    int i = 0, j = 0;
    bool bFind = TRUE;

    this->clearTable();

    while (bFind)
    {
        this->arr[i][j] = this->getRandNum(i, j);
        this->setTableColNumSts(j, this->arr[i][j]);
        this->setTableRowNumSts(i, this->arr[i][j]);
        this->setTableSudoku3x3NumSts(i, j, this->arr[i][j]);
        this->missCnt[i][j] = INVALID_MISSCNT;
        bFind = findMinCntPos(&i, &j);
    }
}

void sudoku::clearTable(void)
{
    (void)memset((void*)this->arr, 0, sizeof(this->arr));
    (void)memset((void*)this->bit, 0, sizeof(this->bit));
    (void)memset((void*)this->missCnt, 0, sizeof(this->missCnt));
}



sudoku::sudoku(void)
{
    this->clearTable();
}

u8_t sudoku::getRandNum(int row, int col)
{
    int count = 0;
    u32_t bitval = this->bit[row][col];
    vector<int> arr;
    
    for (int i = 0; i < NUM_OF_GAMENUM; i++)
    {
        if ((bitval & (1 << i)) == 0)
        {
            arr.push_back(i + 1);
            count++;
        }
    }

    int rnd = arr[rand() % count];
    return rnd;
}

void sudoku::show(void)
{
    for (int i = 0; i < ROW_NUM; i++)
    {
        for (int j = 0; j < COL_NUM; j++)
        {
            std::cout << (int)this->arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void sudoku::setTableRowNumSts(int row, int num)
{
    u32_t bitval = (1 << (num - 1));
    for (int i = 0; i < COL_NUM; i++)
    {
        if ((this->bit[row][i] & bitval) == 0 && this->missCnt[row][i] != INVALID_MISSCNT)
        {
            this->missCnt[row][i]++;
        }
        this->bit[row][i] |= bitval;
    }
}

void sudoku::setTableColNumSts(int col, int num)
{
    u32_t bitval = (1 << (num - 1));
    for (int i = 0; i < ROW_NUM; i++)
    {
        if ((this->bit[i][col] & bitval) == 0 && this->missCnt[i][col] != INVALID_MISSCNT)
        {
            this->missCnt[i][col]++;
        }
        this->bit[i][col] |= bitval;
    }
}

void sudoku::setTableSudoku3x3NumSts(int row, int col, int num)
{
    u32_t bitval = (1 << (num - 1));
    
    row = ((int)(row / 3)) * 3; 
    col = ((int)(col / 3)) * 3;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if ((this->bit[row + i][col + j] & bitval) == 0 && this->missCnt[row + i][col + j] != INVALID_MISSCNT)
            {
                this->missCnt[row + i][col + j]++;
            }
            this->bit[row + i][col + j] |= bitval;
        }
    }
}

bool sudoku::findMinCntPos(int* posrow, int* poscol)
{
    s8_t maxMissCnt = -1;
    int pr, pc;
    for (int i = 0; i < ROW_NUM; i++)
    {
        for (int j = 0; j < COL_NUM; j++)
        {
            if (maxMissCnt < this->missCnt[i][j])
            {
                maxMissCnt = this->missCnt[i][j];
                pr = i;
                pc = j;
            }
        }
    }
    if (maxMissCnt == -1)
    {
        return FALSE;
    }
    else if (maxMissCnt < NUM_OF_GAMENUM)
    {
        *posrow = pr;
        *poscol = pc;
        return TRUE;
    }
    else
    {
        this->clearTable();
        *posrow = 0;
        *poscol = 0;
        return TRUE;
    }
}

int main()
{
    sudoku sudu;
    srand(static_cast<u32_t>(time(NULL)));
    std::cout << (int)sudu.getRandNum(0,0) << endl;
    // test 
    int cnt = 10000;
    while (cnt--)
    {
        sudu.generateTable();
        std::cout << (int)cnt << endl;
    }
    sudu.show();
}

