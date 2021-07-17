// game2048.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <graphics.h>

using namespace std;

#define WIDTH               (720)
#define HEIGHT              (720)
#define SQUARE_LENGTH       (80)
#define SQUARE_GAP          (15)

#define NUM_OF_BLOCK         (4)

#define MAX_GAME_EXPONENT               (11)
//#define PLAYER_TARGET                   (2048)

#define MOVING_LEFT          (1)
#define MOVING_RIGHT         (2)
#define MOVING_UP            (3)
#define MOVING_DOWN          (4)

#define GENERATE2_PROBABILITY   (95) // 95% generate 2, 5% generate 4;

#ifndef TRUE
#define TRUE
#endif
#ifndef FALSE
#define FALSE
#endif


void  createNewBlock(void);
bool  keyControl(void);
void  drawSquare(int x, int y, int length);
float dynTextSizeRatio(int val);
void  drawBlock(void);
void  showVal(void);
bool  combineVal(int mode);
void  gameInit(void);

int blockVal[NUM_OF_BLOCK][NUM_OF_BLOCK];
int blockPos[NUM_OF_BLOCK][2];
RECT blockRect[NUM_OF_BLOCK][NUM_OF_BLOCK];

/* 存储空余区块 */
int* pEmptyBlock[NUM_OF_BLOCK * NUM_OF_BLOCK];
/* 记录空余区块 */
int emptyBlock;

bool bFull = FALSE;
bool bTarget = FALSE;

const COLORREF color[MAX_GAME_EXPONENT + 1] = {

    LIGHTGRAY,
    CYAN,
    LIGHTCYAN,
    BROWN,
    BLUE,
    LIGHTBLUE,
    GREEN,
    LIGHTGREEN,
    MAGENTA,
    LIGHTMAGENTA,
    RED,
    LIGHTRED,
};

const LPCTSTR displayNum[MAX_GAME_EXPONENT + 1] = {

    _T("0"),
    _T("2"),
    _T("4"),
    _T("8"),
    _T("16"),
    _T("32"),
    _T("64"),
    _T("128"),
    _T("256"),
    _T("512"),
    _T("1024"),
    _T("2048"),
};

void drawSquare(int x, int y, int length)
{
    int leftx = x;
    int topy = y;
    int rightx = x + length;
    int bottomy = y + length;
    //fillrectangle(leftx, topy, rightx, bottomy);
    fillroundrect(leftx, topy, rightx, bottomy, 20, 20);
}

/* 动态显示字体大小 */
float dynTextSizeRatio(int val)
{
    float ratio;
    // 1位最大数, 2 ^ 3 = 8
    if (val < 4)
    {
        ratio = 0.9f;
    }
    else if (val < 7)// 2位最大数, 2 ^ 6 = 64
    {
        ratio = 0.8f;
    }
    else if (val < 10)// 3位最大数, 2 ^ 9 = 512
    {
        ratio = 0.7f;
    }
    else
    {
        ratio = 0.5f;
    }
    return ratio;
}

void drawBlock(void)
{
    int i, j;
    /* 画区块 */
    for (i = 0; i < NUM_OF_BLOCK; i++)
    {
        for (j = 0; j < NUM_OF_BLOCK; j++)
        {
            setfillcolor(color[blockVal[i][j]]);
            drawSquare(blockPos[j][0], blockPos[i][1], SQUARE_LENGTH);
        }
    }
}

void showVal(void)
{
    int i, j;
    LOGFONT prefont;
    int preMode;
    int nHeight = SQUARE_LENGTH - SQUARE_GAP * 1;

    /* 记录之前配置 */
    gettextstyle(&prefont);
    preMode = getbkmode();
    /* 设置背景透明色 */
    setbkmode(TRANSPARENT);

    for (i = 0; i < NUM_OF_BLOCK; i++)
    {
        for (j = 0; j < NUM_OF_BLOCK; j++)
        {
            if (blockVal[i][j] != 0)
            {
                settextstyle((int)((float)nHeight * dynTextSizeRatio(blockVal[i][j])), 0, _T("黑体"));
                drawtext(displayNum[blockVal[i][j]], &blockRect[i][j], DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            }

            if (blockVal[i][j] == MAX_GAME_EXPONENT)
            {
                bTarget = TRUE;
            }
        }
    }

    /* 还原之前配置 */
    setbkmode(preMode);
    settextstyle(&prefont);
}

void SetEmptyBlock(void)
{
    int i, j;

    emptyBlock = 0;
    for (i = 0; i < NUM_OF_BLOCK; i++)
    {
        for (j = 0; j < NUM_OF_BLOCK; j++)
        {
            if (blockVal[i][j] == 0)
            {
                pEmptyBlock[emptyBlock++] = &blockVal[i][j];
            }
        }
    }

    if (emptyBlock == 0)
    {
        bFull = TRUE;
    }
}

void createOneNewBlock(void)
{
    int i1;

    srand((unsigned int)time(NULL));

    if (emptyBlock >= 1)
    {
        /* 生成1个随机数 */
        i1 = rand() % emptyBlock;
        int gen2 = rand() % 100;
        if (gen2 < GENERATE2_PROBABILITY)
        {
            *pEmptyBlock[i1] = 1;
        }
        else
        {
            *pEmptyBlock[i1] = 2;
        }
        emptyBlock -= 1;
    }
}

void createTwoNewBlock(void)
{
    if (emptyBlock >= 2)
    {
        /* 生成2个随机数 */

        vector<int> index(emptyBlock, 0);
        for (int i = 0; i < emptyBlock; i++)
        {
            index[i] = i;
        }

        for (int i = 0; i < 2; i++)
        {
            int tmp = rand() % emptyBlock;
            int gen2 = rand() % 100;
            if (gen2 < GENERATE2_PROBABILITY)
            {
                *pEmptyBlock[index[tmp]] = 1;
            }
            else
            {
                *pEmptyBlock[index[tmp]] = 2;
            }
            index[tmp] = emptyBlock;
            emptyBlock--;
        }
#if 0
        int i1, i2;
        i1 = rand() % emptyBlock;
        i2 = rand() % emptyBlock;

        if (i1 == i2)
        {
            if (i1 < (emptyBlock - 1))
            {
                i2 = i1 + 1;
            }
            else
            {
                i2 = 0;
            }
        }

        *pEmptyBlock[i1] = 1;
        *pEmptyBlock[i2] = 1;

        emptyBlock -= 2;
#endif
    }
}


bool combineVal(int mode)
{
    bool ret = TRUE;
    int i, j;

    switch (mode)
    {
    case MOVING_LEFT:
        /* 合并 */
        for (i = 0; i < NUM_OF_BLOCK; i++)
        {
            for (j = 0; j < NUM_OF_BLOCK; j++)
            {
                if (blockVal[i][j] != 0)
                {
                    int k = j + 1;
                    /* 寻找下一个不等于0的值 */
                    while (k < NUM_OF_BLOCK && blockVal[i][k] == 0)
                    {
                        k++;
                    };

                    if (k < NUM_OF_BLOCK && blockVal[i][j] == blockVal[i][k])
                    {
                        blockVal[i][j] += 1;
                        blockVal[i][k] = 0;
                    }
                }
            }
        }

        /* 移动 */
        for (i = 0; i < NUM_OF_BLOCK; i++)
        {
            for (j = 0; j < NUM_OF_BLOCK; j++)
            {
                if (blockVal[i][j] == 0)
                {
                    int k = j + 1;
                    /* 寻找下一个不等于0的值 */
                    while (k < NUM_OF_BLOCK && blockVal[i][k] == 0)
                    {
                        k++;
                    };

                    if (k < NUM_OF_BLOCK)
                    {
                        blockVal[i][j] = blockVal[i][k];
                        blockVal[i][k] = 0;
                    }
                }
            }
        }
        break;

    case MOVING_RIGHT:
        /* 合并 */
        for (i = 0; i < NUM_OF_BLOCK; i++)
        {
            for (j = NUM_OF_BLOCK - 1; j > 0; j--)
            {
                if (blockVal[i][j] != 0)
                {
                    int k = j - 1;
                    /* 寻找下一个不等于0的值 */
                    while (k >= 0 && blockVal[i][k] == 0)
                    {
                        k--;
                    };

                    if (k >= 0 && blockVal[i][j] == blockVal[i][k])
                    {
                        blockVal[i][j] += 1;
                        blockVal[i][k] = 0;
                    }
                }
            }
        }

        /* 移动 */
        for (i = 0; i < NUM_OF_BLOCK; i++)
        {
            for (j = NUM_OF_BLOCK - 1; j > 0; j--)
            {
                if (blockVal[i][j] == 0)
                {
                    int k = j - 1;
                    /* 寻找下一个不等于0的值 */
                    while (k >= 0 && blockVal[i][k] == 0)
                    {
                        k--;
                    };

                    if (k >= 0)
                    {
                        blockVal[i][j] = blockVal[i][k];
                        blockVal[i][k] = 0;
                    }
                }
            }
        }

        break;

    case MOVING_DOWN:        /* 合并 */
        for (i = 0; i < NUM_OF_BLOCK; i++)
        {
            for (j = NUM_OF_BLOCK - 1; j > 0; j--)
            {
                if (blockVal[j][i] != 0)
                {
                    int k = j - 1;
                    /* 寻找下一个不等于0的值 */
                    while (k >= 0 && blockVal[k][i] == 0)
                    {
                        k--;
                    };

                    if (k >= 0 && blockVal[j][i] == blockVal[k][i])
                    {
                        blockVal[j][i] += 1;
                        blockVal[k][i] = 0;
                    }
                }
            }
        }

        /* 移动 */
        for (i = 0; i < NUM_OF_BLOCK; i++)
        {
            for (j = NUM_OF_BLOCK - 1; j > 0; j--)
            {
                if (blockVal[j][i] == 0)
                {
                    int k = j - 1;
                    /* 寻找下一个不等于0的值 */
                    while (k >= 0 && blockVal[k][i] == 0)
                    {
                        k--;
                    };

                    if (k >= 0)
                    {
                        blockVal[j][i] = blockVal[k][i];
                        blockVal[k][i] = 0;
                    }
                }
            }
        }
        break;

    case MOVING_UP:
        /* 合并 */
        for (i = 0; i < NUM_OF_BLOCK; i++)
        {
            for (j = 0; j < NUM_OF_BLOCK; j++)
            {
                if (blockVal[j][i] != 0)
                {
                    int k = j + 1;
                    /* 寻找下一个不等于0的值 */
                    while (k < NUM_OF_BLOCK && blockVal[k][i] == 0)
                    {
                        k++;
                    };

                    if (k < NUM_OF_BLOCK && blockVal[j][i] == blockVal[k][i])
                    {
                        blockVal[j][i] += 1;
                        blockVal[k][i] = 0;
                    }
                }
            }
        }

        /* 移动 */
        for (i = 0; i < NUM_OF_BLOCK; i++)
        {
            for (j = 0; j < NUM_OF_BLOCK; j++)
            {
                if (blockVal[j][i] == 0)
                {
                    int k = j + 1;
                    /* 寻找下一个不等于0的值 */
                    while (k < NUM_OF_BLOCK && blockVal[k][i] == 0)
                    {
                        k++;
                    };

                    if (k < NUM_OF_BLOCK)
                    {
                        blockVal[j][i] = blockVal[k][i];
                        blockVal[k][i] = 0;
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return ret;
}

bool keyControl(void)
{
    bool ret = FALSE;
    int mode = 0;
    if (_kbhit())
    {
        char key = _getch();
        switch (key)
        {
        case 'W':
        case 'w':
            combineVal(MOVING_UP);
            ret = TRUE;
            break;
        case 'A':
        case 'a':
            combineVal(MOVING_LEFT);
            ret = TRUE;
            break;
        case 'S':
        case 's':
            combineVal(MOVING_DOWN);
            ret = TRUE;
            break;
        case 'D':
        case 'd':
            combineVal(MOVING_RIGHT);
            ret = TRUE;
            break;
        default:
            break;
        }
    }
    return ret;
}

void gameStart(void)
{

    BeginBatchDraw();
    drawBlock();
    showVal();
    FlushBatchDraw();

    while (!bTarget && !bFull)
    {
        if (keyControl())
        {
            SetEmptyBlock();
            createOneNewBlock();
            drawBlock();
            showVal();
            FlushBatchDraw();
        }
    }
    if (bTarget)
    {
        printf("you win!\n");
    }
    else if (bFull)
    {
        printf("you lose!\n");
    }

    EndBatchDraw();
}

void gameInit(void)
{
    int i, j;
    int length;
    int stx, sty;
    //initgraph(width, height);
    initgraph(WIDTH, HEIGHT, EW_SHOWCONSOLE);

    /* 初始白色背景*/
    length = SQUARE_LENGTH * NUM_OF_BLOCK + SQUARE_GAP * (NUM_OF_BLOCK + 1);
    stx = WIDTH / 2 - length / 2;
    sty = HEIGHT / 2 - length / 2;
    setfillcolor(WHITE);
    drawSquare(stx, sty, length);

    /* 计算各区块的起始位置 */
    for (i = 0; i < NUM_OF_BLOCK; i++)
    {
        /* x */
        blockPos[i][0] = stx + SQUARE_GAP * (i + 1) + SQUARE_LENGTH * i;
        /* y */
        blockPos[i][1] = sty + SQUARE_GAP * (i + 1) + SQUARE_LENGTH * i;
    }

    for (i = 0; i < NUM_OF_BLOCK; i++)
    {
        for (j = 0; j < NUM_OF_BLOCK; j++)
        {
            blockRect[i][j].left = blockPos[j][0];
            blockRect[i][j].top = blockPos[i][1];
            blockRect[i][j].right = blockPos[j][0] + SQUARE_LENGTH;
            blockRect[i][j].bottom = blockPos[i][1] + SQUARE_LENGTH;
            pEmptyBlock[i * NUM_OF_BLOCK + j] = &blockVal[i][j];
        }
    }

    emptyBlock = NUM_OF_BLOCK * NUM_OF_BLOCK;
    createTwoNewBlock();
}

void gameClose(void)
{
    closegraph();
}

int main()
{
    gameInit();
    gameStart();
    _getch();
    gameClose();
    return 0;
}
