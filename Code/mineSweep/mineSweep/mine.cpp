
#include <iostream>
#include <vector>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include "mine.h"

std::vector<point> dir = { {0, -1},{0, 1},{-1, -1},{-1, 0},{-1, 1},{1, -1},{1, 0},{1,1} };
std::vector<point> dir2 = { {0, -1},{0, 1},{-1, 0},{1, 0} };

Mine::Mine(int nr, int nc, int num)
{
    row = nr;
    col = nc;
    mineNum = num;
    mineMap = std::vector<std::vector<u8_t>>(row, std::vector<u8_t>(col, EMPTY_FLAG));
    mineSurroundCount = std::vector<std::vector<u8_t>>(row, std::vector<u8_t>(col, 0));
    ShowMap = std::vector<std::vector<u8_t>>(row, std::vector<u8_t>(col, 0));
}

void Mine::CalcSurroundMineNum()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            mineSurroundCount[i][j] = 0;
            point temp(i, j);
            for (auto d : dir)
            {
                point rst = temp + d;
                if (IsInMap(rst) && IsMine(rst) == true)
                {
                    mineSurroundCount[i][j]++;
                }
            }
        }
    }
}

void Mine::createMine(point p)
{
    int sumCell = row * col;
    srand(static_cast<unsigned>(time(NULL)));
    std::vector<int> indexArr(sumCell);

    for (int i = 0; i < sumCell; i++)
    {
        indexArr[i] = i;
    }

    if (IsInMap(p))
    {
        std::swap(indexArr[p.x * col + p.y], indexArr[sumCell - 1]);
        sumCell--;
        for (auto d : dir2)
        {
            point t2 = p + d;
            if (IsInMap(t2))
            {
                std::swap(indexArr[t2.x * col + t2.y], indexArr[sumCell - 1]);
                sumCell--;
            }
        }
    }

    for (int i = 0; i < mineNum; i++)
    {
        int temp = rand() % (sumCell - i);
        int r = indexArr[temp] / col;
        int c = indexArr[temp] % col;
        mineMap[r][c] = MINE_FLAG;
        std::swap(indexArr[temp], indexArr[sumCell - i - 1]);
    }
    CalcSurroundMineNum();
}

u8_t Mine::getMineVal(int ir, int ic)
{
    return mineMap[ir][ic];
}

u8_t Mine::getMineVal(point p)
{
    return mineMap[p.x][p.y];
}

u8_t Mine::getSurroundMineNum(int ir, int ic)
{
    return mineSurroundCount[ir][ic];
}

u8_t Mine::getSurroundMineNum(point p)
{
    return mineSurroundCount[p.x][p.y];
}

bool Mine::IsMine(int ir, int ic)
{
    return mineMap[ir][ic] == MINE_FLAG;
}

bool Mine::IsMine(point p)
{
    return mineMap[p.x][p.y] == MINE_FLAG;
}

bool Mine::IsInMap(int ir, int ic)
{
    if (ir >= 0 && ir < row && ic >= 0 && ic < col)
    {
        return true;
    }
    return false;
}

bool Mine::IsInMap(point p)
{
    if (p.x >= 0 && p.x < row && p.y >= 0 && p.y < col)
    {
        return true;
    }
    return false;
}



void Mine::updateShowMap(int r, int c)
{
    std::queue<point> q;
    q.push(point(r, c));
    ShowMap[r][c] = VISITED_NUM;

    while (!q.empty())
    {
        point cur = q.front();
        q.pop();
        std::vector<point> curdir;
        if (mineSurroundCount[cur.x][cur.y] == 0)
        {
            ShowMap[cur.x][cur.y] = VISITED;
            curdir = std::vector<point>(dir);
        }
        else
        {
            curdir = std::vector<point>(dir2);
        }

        for (auto d : curdir)
        {
            point temp = cur + d;
            if (IsInMap(temp) && ShowMap[temp.x][temp.y] == NOT_VISIT && IsMine(temp) == false)
            {
                if (mineSurroundCount[temp.x][temp.y] != 0)
                {
                    ShowMap[temp.x][temp.y] = VISITED_NUM;
                }
                else
                {
                    q.push(temp);
                    ShowMap[temp.x][temp.y] = VISITED;
                }
            }
        }
    }

    //更新是否显示数字
#if 0
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (i != r && j != c && ShowMap[i][j] == VISITED_NUM)
            {
                bool bfind = false;
                point cur(i, j);

                //for (auto d : dir2)
                //{
                //    point temp = cur + d;
                //    if (IsInMap(temp) && ShowMap[temp.x][temp.y] != NOT_VISIT)
                //    {
                //        bfind = true;
                //        break;
                //    }
                //}

                //if (bfind != true)
                //{
                //    ShowMap[i][j] = NOT_VISIT;
                //    continue;
                //}

                bfind = false;
                for (auto d : dir)
                {
                    point temp = cur + d;
                    if (IsInMap(temp) && ShowMap[temp.x][temp.y] == NOT_VISIT)
                    {
                        bfind = true;
                        break;
                    }
                }
                if (bfind == false)
                {
                    ShowMap[i][j] = VISITED;
                }


            }
        }
    }
#endif
}

bool Mine::click(int r, int c)
{
    if (IsMine(r, c))
    {
        return false;
    }
    else
    {
        if (ShowMap[r][c] == NOT_VISIT)
        {
            updateShowMap(r, c);
        }
        return true;
    }
}

bool Mine::IsSuccess()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (ShowMap[i][j] == NOT_VISIT && IsMine(i, j) != TRUE)
            {
                return false;
            }
        }
    }
    return true;
}

void Mine::showMine()
{
    system("cls");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            //out << (int)myMine.getMineVal(i, j) << " ";
            switch (ShowMap[i][j])
            {
            case NOT_VISIT:
                std::cout << "*";
                break;
            case VISITED:
                std::cout << "-";
                break;
            case VISITED_NUM:
                std::cout << (int)getSurroundMineNum(i, j);
                break;
            default:
                std::cout << (int)getMineVal(i, j);
                break;
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, Mine& myMine)
{
    system("cls");
    for (int i = 0; i < myMine.row; i++)
    {
        for (int j = 0; j < myMine.col; j++)
        {
            //out << (int)myMine.getMineVal(i, j) << " ";
            switch (myMine.ShowMap[i][j])
            {
            case NOT_VISIT:
                out << "*";
                break;
            case VISITED:
                out << "-";
                break;
            case VISITED_NUM:
                out << (int)myMine.getSurroundMineNum(i, j);
                break;
            default:
                break;
            }
            out << " ";
        }
        out << std::endl;
    }
    return out;
}
