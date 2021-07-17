/*
    blockCrash.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
    模拟方块与方块之间的碰撞或者方块和墙壁的碰撞反弹
    假设:
    1, 碰撞不会导致能量损耗
    2, 为简化模型，矩形的竖线接触，采用水平弹性碰撞, 矩形的水平线线接触，采用垂直弹性碰撞， 顶点碰撞水平和垂直同时处理
*/

#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <math.h>

#define WIDTH          (960)
#define HEIGHT         (720)

#define NUM_OF_RECT    (10)

#define NORMAL_COLOR   (BLUE)
#define CRASH_COLOR    (RED)

#define MIN_WIDTH      (10)
#define MAX_WIDTH      (100)
#define MIN_HEIGHT     (10)
#define MAX_HEIGHT     (100)

#define MAX_SPEED      (10)
#define CYCLE_TIME     (20)
#define VEL_SCALING    (1)

#define EPSILON        (1e-06f)

typedef enum 
{
    NONCRASH = 0,
    CRASH,
    FREE,
}crashStatus_t;

typedef struct
{
    int x;
    int y;
    float fx;
    float fy;
    float vx;
    float vy;
    float cacheVx;
    float cacheVy;
    int height;
    int width;
    crashStatus_t cs;
}blockRect_t;


bool IsRectOverLap(blockRect_t *prect1, blockRect_t* prect2); 

void clearRect(void);
void drawRect(void);

blockRect_t rectArr[NUM_OF_RECT];

void creatTestRect(void)
{
    rectArr[0].x = 460;
    rectArr[0].y = 380;
    rectArr[0].width = 100;
    rectArr[0].height = 100;
    rectArr[0].vx = 5;
    rectArr[0].vy = 5;
    rectArr[0].cacheVx = rectArr[0].vx;
    rectArr[0].cacheVy = rectArr[0].vy;
    rectArr[0].fx = (float)rectArr[0].x;
    rectArr[0].fy = (float)rectArr[0].y;
    rectArr[0].cs = NONCRASH;

    rectArr[1].x = 370;
    rectArr[1].y = 490;
    rectArr[1].width = 100;
    rectArr[1].height = 100;
    rectArr[1].vx = -5;
    rectArr[1].vy = -5;
    rectArr[1].cacheVx = rectArr[1].vx;
    rectArr[1].cacheVy = rectArr[1].vy;
    rectArr[1].fx = (float)rectArr[1].x;
    rectArr[1].fy = (float)rectArr[1].y;
    rectArr[1].cs = NONCRASH;
}

bool IsRectOverLap(blockRect_t* prect1, blockRect_t* prect2)
{
    bool ret = FALSE;
    int sub1, sub2;

    /* 构造矩形 */
    if (prect1->x < prect2->x)
    {
        sub1 = prect2->x - (prect1->x + prect1->width);
    }
    else
    {
        sub1 = prect1->x - (prect2->x + prect2->width);
    }

    if (prect1->y < prect2->y)
    {
        sub2 = prect2->y - (prect1->y + prect1->height);
    }
    else
    {
        sub2 = prect1->y - (prect2->y + prect2->height);
    }

    if (sub1 < 0 && sub2 < 0)
    {
        ret = TRUE;
    }
    return ret;
}

void initRect(void)
{
    bool bFound;
    int i, j;
    blockRect_t temp;

    memset(rectArr, 0, sizeof(rectArr));
    srand(static_cast<unsigned int>(time(NULL)));

    for ( i = 0; i < NUM_OF_RECT; i++)
    {
        bFound = TRUE;
        while (bFound == TRUE)
        {
            bFound = FALSE;

            /* 随机生成矩形 */
            temp.x = rand() % (WIDTH - MAX_WIDTH);
            temp.y = rand() % (HEIGHT - MAX_HEIGHT);
            temp.width = MIN_WIDTH + rand() % (MAX_WIDTH - MIN_WIDTH);
            temp.height = MIN_HEIGHT + rand() % (MAX_HEIGHT - MIN_HEIGHT);

            /* 判断是否和其他已生成好的矩形重叠 */
            for (j = 0; j < i && bFound == FALSE; j++)
            {
                if (IsRectOverLap(&temp, &rectArr[j]))
                {
                    bFound = TRUE;
                }
            }
        }
        temp.vx = (float)(rand() % MAX_SPEED) / VEL_SCALING;
        temp.vy = (float)(rand() % MAX_SPEED) / VEL_SCALING;
        temp.cacheVx = temp.vx;
        temp.cacheVy = temp.vy;
        temp.fx = (float)temp.x;
        temp.fy = (float)temp.y;
        temp.cs = NONCRASH;
        rectArr[i] = temp;
    }
}

void Init(void)
{
    initgraph(WIDTH, HEIGHT);

    /* 初始化各个矩形的坐标 */
    initRect();
    //creatTestRect();

    /* 显示矩形 */
    drawRect();
}

void Close(void)
{
    closegraph();
}

void CalcVel(float* v1, float* v2, float m1, float m2)
{
    float prev1, prev2, subm, summ; 

    prev1 = *v1;
    prev2 = *v2;
    subm = m1 - m2;
    summ = m1 + m2;

    *v1 = (subm * prev1 + 2 * m2 * prev2) / summ;
    *v2 = (subm * prev2 + 2 * m1 * prev1) / summ;
}

bool IsReverse(float v1, float v2)
{
    bool ret = FALSE;

    if (v1 * v2 <= 0 && v1 != 0)
    {
        ret = TRUE;
    }
    else if (fabsf(v1) > fabsf(v2))
    {
        ret = TRUE;
    }

    return ret;
}

void CalcDiagonalCrashVel(blockRect_t* prect1, blockRect_t* prect2, float subx, float suby, float subvx, float subvy)
{
    float m1, m2;
    float t1 = fabsf(subx / (fabsf(subvx) + EPSILON));
    float t2 = fabsf(suby / (fabsf(subvy) + EPSILON));

    m1 = (float)(prect1->width * prect1->height);
    m2 = (float)(prect2->width * prect2->height);

    if (t1 < t2)
    {
        /* 竖线先碰到, 水平弹性碰撞 */
        //CalcVel(&prect1->vy, &prect2->vy, m1, m2);
        if (IsReverse(prect1->cacheVx, prect2->cacheVx))
        {
            prect1->vx *= -1;
        }
    }
    else if (t1 > t2)
    {
        /* 横线先碰到, 垂直弹性碰撞 */
        //CalcVel(&prect1->vx, &prect2->vx, m1, m2);
        if (IsReverse(prect1->cacheVy, prect2->cacheVy))
        {
            prect1->vy *= -1;
        }
    }
    else
    {
        /* 顶点相撞 */
        //CalcVel(&prect1->vx, &prect2->vx, m1, m2);
        //CalcVel(&prect1->vy, &prect2->vy, m1, m2);

        if (IsReverse(prect1->cacheVx, prect2->cacheVx))
            prect1->vx *= -1;
        if (IsReverse(prect1->cacheVy, prect2->cacheVy))
            prect1->vy *= -1;
    }
}

void CrashByBoundary(blockRect_t* prect)
{
    if ((prect->x <= 0 && prect->vx < 0) || (prect->x + prect->width >= WIDTH && prect->vx > 0))
    {
        prect->vx *= -1;
    }

    if ((prect->y <= 0 && prect->vy < 0) || (prect->y + prect->height >= HEIGHT && prect->vy > 0))
    {
        prect->vy *= -1;
    }
}

void CrashByRect(blockRect_t* prect1, blockRect_t* prect2)
{

    bool xFlag = FALSE, yFlag = FALSE;
    float subx, suby, subvx, subvy;
    blockRect_t* base, * side;
    /* 计算重合差量 */
    if (prect1->fx < prect2->fx)
    {
        base = prect1;
        side = prect2;
    }
    else
    {
        base = prect2;
        side = prect1;
    }

    subx = side->fx - (base->fx + base->width);

    if (prect1->fy < prect2->fy)
    {
        base = prect1;
        side = prect2;
    }
    else
    {
        base = prect2;
        side = prect1;
    }

    suby = side->fy - (base->fy + base->height);

    /* 简单判断是否有重叠 */
    if (subx <= 0 && suby <= 0)
    {
        /* 判断1在2的什么方位， 左边，左上， 上边， 右上，右边， 右下， 下边， 左下 */
        if (fabsf(prect1->fx - prect2->fx) < EPSILON/* 横坐标相等 */
        || (prect1->fx < prect2->fx && (prect1->fx + prect1->width > prect2->fx + prect2->width))/* 横轴方向 2 被 1 包含 */
        || (prect2->fx < prect1->fx && (prect2->fx + prect2->width > prect1->fx + prect1->width)) /* 横轴方向 1 被 2 包含 */)
        {
            if (prect1->fy < prect2->fy)
            {
                /* 1在2的上边 */
                if ((prect1->cacheVy > 0 && prect2->cacheVy < EPSILON)/* 垂直对向碰撞 */ || (prect1->cacheVy < 0 && prect1->cacheVy > prect2->cacheVy)/* 垂直同向碰撞 */)
                {
                    prect1->vy *= -1;
                }
            }
            else
            {
                /* 1在2的下边 */
                if ((prect1->cacheVy < 0 && prect2->cacheVy > -EPSILON)/* 垂直对向碰撞 */ || (prect1->cacheVy > 0 && prect1->cacheVy < prect2->cacheVy)/* 垂直同向碰撞 */)
                {
                    prect1->vy *= -1;
                }
            }
            /* 碰撞状态 */
            prect1->cs = CRASH;
        }
        else if (fabsf(prect1->fy - prect2->fy) < EPSILON/* 纵坐标相等 */
            || (prect1->fy < prect2->fy && (prect1->fy + prect1->height > prect2->fy + prect2->height))/* 纵轴方向 2 被 1 包含 */
            || (prect2->fy < prect1->fy && (prect2->fy + prect2->height > prect1->fy + prect1->height)) /* 纵轴方向 1 被 2 包含 */)
        {
            if (prect1->fx < prect2->fx)
            {
                /* 1在2的左边 */
                if ((prect1->cacheVx > 0 && prect2->cacheVx < EPSILON)/* 水平对向碰撞 */ || (prect1->cacheVx < 0 && prect1->cacheVx > prect2->cacheVx)/* 水平同向碰撞 */)
                {
                    prect1->vx *= -1;
                }
            }
            else
            {
                /* 1在2的右边 */
                if ((prect1->cacheVx < 0 && prect2->cacheVx > -EPSILON)/* 水平对向碰撞 */ || (prect1->cacheVx > 0 && prect1->cacheVx < prect2->cacheVx)/* 水平同向碰撞 */)
                {
                    prect1->vx *= -1;
                }
            }
            /* 碰撞状态 */
            prect1->cs = CRASH;
        }
        else if ((prect1->fx < prect2->fx && (prect1->fx + prect1->width < prect2->fx + prect2->width))/* 1位于2的左半部分 */)
        {
            if (prect1->fy < prect2->fy)
            {
                /* 1在2的左上 */
                subvx = prect1->cacheVx - prect2->cacheVx;
                subvy = prect1->cacheVy - prect2->cacheVy;
                if (fabsf(subx) < EPSILON && fabsf(suby) < EPSILON && (subvx < EPSILON || subvy < EPSILON))
                {
                    /* 非碰撞情形 */
                    prect1->cs = NONCRASH;
                }
                else
                {
                    if (subvx < 0)
                    {
                        subx = prect1->fx - prect2->fx;
                    }
                    if (subvy < 0)
                    {
                        suby = prect1->fy - prect2->fy;
                    }
                    CalcDiagonalCrashVel(prect1, prect2, subx, suby, subvx, subvy);
                    /* 碰撞状态 */
                    prect1->cs = CRASH;
                }
            }
            else
            {
                /* 1在2的左下 */
                subvx = prect1->cacheVx - prect2->cacheVx;
                subvy = prect1->cacheVy - prect2->cacheVy;
                if (fabsf(subx) < EPSILON && fabsf(suby) < EPSILON && (subvx < EPSILON || subvy > -EPSILON))
                {
                    /* 非碰撞情形 */
                    prect1->cs = NONCRASH;
                }
                else
                {
                    if (subvx < 0)
                    {
                        subx = prect1->fx - prect2->fx;
                    }
                    if (subvy > 0)
                    {
                        suby = prect2->fy - prect1->fy;
                    }
                    CalcDiagonalCrashVel(prect1, prect2, subx, suby, subvx, subvy);
                    /* 碰撞状态 */
                    prect1->cs = CRASH;
                }

            }
        }
        else /* 1位于2的右半部分 */
        {
            if (prect1->fy < prect2->fy)
            {
                /* 1在2的右上 */                
                subvx = prect1->cacheVx - prect2->cacheVx;
                subvy = prect1->cacheVy - prect2->cacheVy;
                if (fabsf(subx) < EPSILON && fabsf(suby) < EPSILON && (subvx > -EPSILON || subvy < EPSILON))
                {
                    /* 非碰撞情形 */
                    prect1->cs = NONCRASH;
                }
                else
                {
                    if (subvx > 0)
                    {
                        subx = prect2->fx - prect1->fx;
                    }

                    if (subvy < 0)
                    {
                        suby = prect1->fy - prect2->fy;
                    }
                    CalcDiagonalCrashVel(prect1, prect2, subx, suby, subvx, subvy);
                    /* 碰撞状态 */
                    prect1->cs = CRASH;
                }
            }
            else
            {
                /* 1在2的右下 */
                subvx = prect1->cacheVx - prect2->cacheVx;
                subvy = prect1->cacheVy - prect2->cacheVy;
                if (fabsf(subx) < EPSILON && fabsf(suby) < EPSILON && (subvx > -EPSILON || subvy > -EPSILON))
                {
                    /* 非碰撞情形 */
                    prect1->cs = NONCRASH;
                }
                else
                {
                    if (subvx > 0)
                    {
                        subx = prect2->fx - prect1->fx;
                    }
                    if (subvy > 0)
                    {
                        suby = prect2->fy - prect1->fy;
                    }
                    CalcDiagonalCrashVel(prect1, prect2, subx, suby, subvx, subvy);
                    /* 碰撞状态 */
                    prect1->cs = CRASH;
                }
            }
        }
    }

}

void updateRect(void)
{
    int i, j;
    blockRect_t *prect;
    prect = &rectArr[0];


    prect = &rectArr[0];
    for (i = 0; i < NUM_OF_RECT; i++)
    {
        prect->fx += prect->vx;
        prect->fy += prect->vy;
        prect->x = int(prect->fx + 0.5f);
        prect->y = int(prect->fy + 0.5f);
        /* 边界检测 */
        CrashByBoundary(prect);
        prect++;
    }

    prect = &rectArr[0];
    for (i = 0; i < NUM_OF_RECT; i++)
    {
        /* 碰撞状态切换 */
        switch (prect->cs)
        {
        case NONCRASH:
            for (j = 0; j < NUM_OF_RECT && prect->cs != CRASH; j++)
            {
                if (j != i)
                {
                    CrashByRect(prect, &rectArr[j]);
                }
            }
            break;
        case CRASH:
            break;
        case FREE:
            break;
        default:
            break;
        }
        prect++;
    }

    prect = &rectArr[0];
    for (i = 0; i < NUM_OF_RECT; i++)
    {
        prect->cacheVx = prect->vx;
        prect->cacheVy = prect->vy;
        prect++;
    }

}

void clearRect(void)
{
    int i;
    blockRect_t* prect = &rectArr[0];
    COLORREF bk = getbkcolor();
    for (i = 0; i < NUM_OF_RECT; i++)
    {
        setcolor(bk);
        setfillcolor(bk);
        fillrectangle(prect->x, prect->y, prect->x + prect->width, prect->y + prect->height);
        prect++;
    }
}

void drawRect(void)
{
    int i;
    blockRect_t* prect = &rectArr[0];
    for (i = 0; i < NUM_OF_RECT; i++)
    {
        if (prect->cs == NONCRASH)
        {
            setcolor(NORMAL_COLOR);
            setfillcolor(NORMAL_COLOR);
        }
        else
        {
            setcolor(CRASH_COLOR);
            setfillcolor(CRASH_COLOR);
        }
        fillrectangle(prect->x, prect->y, prect->x + prect->width, prect->y + prect->height);

        prect->cs = NONCRASH;

        prect++;
    }


    LOGFONT prefont;
    RECT temp;
    int preMode;
    WCHAR str[10];
    
    /* 记录之前配置 */
    gettextstyle(&prefont);
    preMode = getbkmode();
    /* 设置背景透明色 */
    setbkmode(TRANSPARENT);
    setcolor(WHITE);
    prect = &rectArr[0];
    for (i = 0; i < NUM_OF_RECT; i++)
    {
        temp.left = prect->x;
        temp.top = prect->y;
        temp.right = prect->x + prect->width;
        temp.bottom = prect->y + prect->height;

        settextstyle(MIN_WIDTH * 2, 0, _T("黑体"));
        wsprintf(str, _T("%d"), i);
        drawtext(str, &temp, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        prect++;
    }

    /* 还原之前配置 */
    setbkmode(preMode);
    settextstyle(&prefont);
}

bool gStart = FALSE;
bool gExit = FALSE;

bool keyEventPause(void)
{
    if (_kbhit())
    {
        char c = _getch();
        //if (c == 's' || c == 'S')
        if (c == ' ')
        {
            return TRUE;
        }
    }
    return FALSE;
}

bool keyEventStart(void)
{
    if (_kbhit())
    {
        char c = _getch();
        if (c == ' ')
        {
            return TRUE;
        }
    }
    return FALSE;
}

void start(void)
{
    static clock_t st = 0, et = 0;
    while (!gStart)
    {
        if (keyEventStart())
        {
            gStart = TRUE;
        }
    }

    BeginBatchDraw();
    while (!gExit)
    {
        st = clock();
        clearRect();
        updateRect();
        drawRect();
        FlushBatchDraw();
        do 
        {
            if (keyEventPause())
            {
                while (!keyEventStart());
            }
            et = clock();
        } while (et - st < CYCLE_TIME);
    }
    EndBatchDraw();
}

int main()
{
    Init(); 
    start();
    Close();
    return 0;
}
