// fishEating.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <MMSystem.h>
#include "Vfw.h"

#pragma comment (lib, "Vfw32.lib")

#pragma comment(lib,"Winmm.lib")

#define WINDOW_WIDTH                (1000)
#define WINDOW_LENGTH               (640)
#define WINDOW_GAP                  (40)
#define WINDOW_BLOCK_SIZE           (80)

#define CLASS_NUM_OF_FISH                 (5)
#define CLASS_NUM_OF_TO0L                 (2)

#define IMG_SRC_LEFT_IDX            (2)
#define IMG_SRC_RIGHT_IDX           (0)
#define TOOL_IDX_OFFSET             (CLASS_NUM_OF_FISH)

#define MOVE_RIGHT                  (0)
#define MOVE_LEFT                   (1)
#define FISH_START_OFFSET           (200)

#define CYCLE_MS                    (16)

using namespace std;

void playMv(void);

int a = (SRCINVERT, NOTSRCERASE);
IMAGE img_bk; 
IMAGE img_enter;
IMAGE img_player[4];
IMAGE img_nfood[5][4];
IMAGE img_sfood[2][4];

typedef struct
{
    int width;//宽度
    int height;//高度
}fishImgInfo_t;

fishImgInfo_t fishImgInfo[CLASS_NUM_OF_FISH + CLASS_NUM_OF_TO0L] =
{
    {30, 20},
    {40, 25},
    {60, 45},
    {110, 60},
    {160, 70},
    {30, 30},
    {30, 30},
};

#define NUM_OF_BLOCK               ((WINDOW_LENGTH- 2 * WINDOW_GAP) / WINDOW_BLOCK_SIZE)

int blockDir[NUM_OF_BLOCK];
int blockFoodCnt[NUM_OF_BLOCK];

typedef struct
{
    int size;//大小
    int vx;//x轴速度
    int vy;//y轴速度
}fish_t;

fish_t fishInfo[CLASS_NUM_OF_FISH + CLASS_NUM_OF_TO0L + 1] = {
    {0, 4, 0},
    {0, 7, 0},
    {0, 6, 0},
    {0, 6, 0},
    {0, 5, 0},
    {0, 2, 0},
    {0, 2, 0},
    {0, 10, 20},
};

typedef struct
{
    int id;
    int posx;//x坐标
    int posy;//y坐标
    int dir;//朝向
    int blockSeq;
}food_t;

typedef struct foodListTag
{
    food_t food;
    struct foodListTag* next;
}foodList_t;

foodList_t *foodHead;

void DrawFood(IMAGE* pFood, int x, int y)
{
    putimage(x, y, &pFood[0], NOTSRCERASE);
    putimage(x, y, &pFood[1], SRCINVERT);
}

void resizePlayer(int x, int y)
{

    loadimage(&img_player[0], _T("./source/playerbei.jpg"), x, y, TRUE);
    loadimage(&img_player[1], _T("./source/player.jpg"), x, y, TRUE);
    loadimage(&img_player[2], _T("./source/f_playerbei.jpg"), x, y, TRUE);
    loadimage(&img_player[3], _T("./source/f_player.jpg"), x, y, TRUE);
}

void Init()
{
    //加载背景图
    loadimage(&img_bk,     _T("./source/background.jpg"), WINDOW_WIDTH, WINDOW_LENGTH, TRUE);
    loadimage(&img_enter,  _T("./source/enter.jpg"), WINDOW_WIDTH, WINDOW_LENGTH, TRUE);

    //加载玩家图
    loadimage(&img_player[0], _T("./source/playerbei.jpg"), 35, 25, TRUE);
    loadimage(&img_player[1], _T("./source/player.jpg"), 35, 25, TRUE);
    loadimage(&img_player[2], _T("./source/f_playerbei.jpg"), 35, 25, TRUE);
    loadimage(&img_player[3], _T("./source/f_player.jpg"), 35, 25, TRUE);

    //加载食物图
    loadimage(&img_nfood[0][0], _T("./source/food1bei.jpg"), fishImgInfo[0].width, fishImgInfo[0].height, TRUE);
    loadimage(&img_nfood[0][1], _T("./source/food1.jpg"), fishImgInfo[0].width, fishImgInfo[0].height, TRUE);
    loadimage(&img_nfood[0][2], _T("./source/f_food1bei.jpg"), fishImgInfo[0].width, fishImgInfo[0].height, TRUE);
    loadimage(&img_nfood[0][3], _T("./source/f_food1.jpg"), fishImgInfo[0].width, fishImgInfo[0].height, TRUE);

    loadimage(&img_nfood[1][0], _T("./source/food2bei.jpg"), fishImgInfo[1].width, fishImgInfo[1].height, TRUE);
    loadimage(&img_nfood[1][1], _T("./source/food2.jpg"), fishImgInfo[1].width, fishImgInfo[1].height, TRUE);
    loadimage(&img_nfood[1][2], _T("./source/f_food2bei.jpg"), fishImgInfo[1].width, fishImgInfo[1].height, TRUE);
    loadimage(&img_nfood[1][3], _T("./source/f_food2.jpg"), fishImgInfo[1].width, fishImgInfo[1].height, TRUE);

    loadimage(&img_nfood[2][0], _T("./source/food3bei.jpg"), fishImgInfo[2].width, fishImgInfo[2].height, TRUE);
    loadimage(&img_nfood[2][1], _T("./source/food3.jpg"), fishImgInfo[2].width, fishImgInfo[2].height, TRUE);
    loadimage(&img_nfood[2][2], _T("./source/f_food3bei.jpg"), fishImgInfo[2].width, fishImgInfo[2].height, TRUE);
    loadimage(&img_nfood[2][3], _T("./source/f_food3.jpg"), fishImgInfo[2].width, fishImgInfo[2].height, TRUE);

    loadimage(&img_nfood[3][0], _T("./source/food4bei.jpg"), fishImgInfo[3].width, fishImgInfo[3].height, TRUE);
    loadimage(&img_nfood[3][1], _T("./source/food4.jpg"), fishImgInfo[3].width, fishImgInfo[3].height, TRUE);
    loadimage(&img_nfood[3][2], _T("./source/f_food4bei.jpg"), fishImgInfo[3].width, fishImgInfo[3].height, TRUE);
    loadimage(&img_nfood[3][3], _T("./source/f_food4.jpg"), fishImgInfo[3].width, fishImgInfo[3].height, TRUE);

    loadimage(&img_nfood[4][0], _T("./source/food5bei.jpg"), fishImgInfo[4].width, fishImgInfo[4].height, TRUE);
    loadimage(&img_nfood[4][1], _T("./source/food5.jpg"), fishImgInfo[4].width, fishImgInfo[4].height, TRUE);
    loadimage(&img_nfood[4][2], _T("./source/f_food5bei.jpg"), fishImgInfo[4].width, fishImgInfo[4].height, TRUE);
    loadimage(&img_nfood[4][3], _T("./source/f_food5.jpg"), fishImgInfo[4].width, fishImgInfo[4].height, TRUE);

    loadimage(&img_sfood[0][0], _T("./source/timebei.jpg"), fishImgInfo[5].width, fishImgInfo[5].height, TRUE);
    loadimage(&img_sfood[0][1], _T("./source/time.jpg"), fishImgInfo[5].width, fishImgInfo[5].height, TRUE);

    loadimage(&img_sfood[1][0], _T("./source/zhadan1bei.jpg"), fishImgInfo[6].width, fishImgInfo[6].height, TRUE);
    loadimage(&img_sfood[1][1], _T("./source/zhadan1.jpg"), fishImgInfo[6].width, fishImgInfo[6].height, TRUE);


    initgraph(WINDOW_WIDTH, WINDOW_LENGTH);
    putimage(0, 0, &img_bk, SRCCOPY);

    /* 初始化背景音乐 */
    mciSendString(_T("open ./source/music.mp3 alias bkmusic"), NULL, 0, NULL);//打开背景音乐
    mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);  // 循环播放

    //playMv();
    foodHead = new foodList_t;
    memset(foodHead, 0, sizeof(foodList_t));

}

int getFoodListSize(void)
{
    int cnt = 0;
    foodList_t* pFood = foodHead;
    while (pFood->next != NULL)
    {
        pFood = pFood->next;
        cnt++;
    }
    return cnt;
}

void show(void)
{
    foodList_t* pFood = foodHead;
    // 初始化背景
    putimage(0, 0, &img_bk, SRCCOPY);
    //画鱼
    while (pFood->next != NULL)
    {
        pFood = pFood->next;
        if (pFood->food.id < CLASS_NUM_OF_FISH)
        {
            if (pFood->food.dir == 1)
            {
                DrawFood(&img_nfood[pFood->food.id][IMG_SRC_RIGHT_IDX], pFood->food.posx, pFood->food.posy);
            }
            else
            {
                DrawFood(&img_nfood[pFood->food.id][IMG_SRC_LEFT_IDX], pFood->food.posx, pFood->food.posy);
            }
        }
    }
}

void updateFoodList(void)
{
    foodList_t* pFood = foodHead;
    int id, dir;
    while (pFood->next != NULL)
    {
        pFood = pFood->next;
        id = pFood->food.id;
        dir = pFood->food.dir;
        pFood->food.posx += fishInfo[id].vx * dir;
    }
}

void createNewFood(int blockSeq)
{
    foodList_t* pnew = new foodList_t;
    memset(pnew, 0, sizeof(foodList_t));

    pnew->food.id = rand()%CLASS_NUM_OF_FISH;
    pnew->food.blockSeq = blockSeq;
    if (blockDir[blockSeq] == MOVE_RIGHT)
    {
        pnew->food.posx = -FISH_START_OFFSET;
        pnew->food.dir = 1;
    }
    else
    {
        pnew->food.posx = (WINDOW_WIDTH + FISH_START_OFFSET);
        pnew->food.dir = -1;
    }
    pnew->food.posy = WINDOW_GAP + blockSeq * WINDOW_BLOCK_SIZE;
    pnew->next = foodHead->next;
    foodHead->next = pnew;
    blockFoodCnt[blockSeq]++;
}

void deleteBoundryFood(void)
{
    foodList_t* pFood = foodHead;
    while (pFood && pFood->next != NULL)
    {
        foodList_t* temp = pFood->next;
        if (temp->food.posx < -FISH_START_OFFSET || temp->food.posx > (WINDOW_WIDTH + FISH_START_OFFSET))
        {
            pFood->next = pFood->next->next;
            blockFoodCnt[temp->food.blockSeq]--;
            delete temp;
        }
        pFood = pFood->next;
    }

}

void playMusic(bool status)
{
    mciSendString(_T("stop gemusic"), NULL, 0, NULL);   // 先把前面一次的音乐停止
    mciSendString(_T("close gemusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
    if (status == TRUE)
    {
        mciSendString(_T("open ./source/1.mp3 alias gemusic"), NULL, 0, NULL); // 打开跳动音乐
    }
    else
    {
        mciSendString(_T("open ./source/2.mp3 alias gemusic"), NULL, 0, NULL); // 打开跳动音乐
    }
    mciSendString(_T("play gemusic"), NULL, 0, NULL); // 仅播放一次
}

void playMv(void)
{
    HWND hwnd = MCIWndCreate(GetHWnd(), NULL, WS_CHILD | WS_VISIBLE | MCIWNDF_NOMENU | MCIWNDF_NOPLAYBAR, NULL);
    SetWindowPos(hwnd, HWND_TOP, 100, 100, 256, 256, SWP_SHOWWINDOW);
    MCIWndOpen(hwnd, _T("C:\\FFOutput\\jade.wmv"), NULL);
    int length = MCIWndGetLength(hwnd);
    MCIWndPlay(hwnd);
    while (TRUE)
    {
        if (_kbhit())
        {
            break;
        }
    }
    MCIWndClose(hwnd);
}

void game(void)
{
    bool exit = FALSE;
    static clock_t start_ms, finish_ms;
    start_ms = clock();
    BeginBatchDraw();
    srand(static_cast<unsigned int>(time(NULL)));
    while (!exit)
    {
        for (int i = 0; i < NUM_OF_BLOCK; i++)
        {
            int rm = rand() % 10;
            if (blockFoodCnt[i] == 0 && rm < 7)
            {
                createNewFood(i);
            }
        }
        updateFoodList();
        deleteBoundryFood();
        show();
        FlushBatchDraw();
        do
        {
            finish_ms = clock();
        } while ((finish_ms - start_ms) < CYCLE_MS);
        start_ms = finish_ms;
    }
    EndBatchDraw();
}

int main()
{
    Init();
    //game();
    _getch();
    closegraph();
}

