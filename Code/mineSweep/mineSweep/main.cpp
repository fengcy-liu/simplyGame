#include <iostream>
#include <string>
#include "mine.h"
#include "render.h"
#include <assert.h>
#include <conio.h>


#define WIDTH               (1280)
#define HEIGHT              (720)
#define BOX_WIDTH           (20)

#define MESS_WIDTH               (1280)
#define MESS_HEIGHT              (720)

class game
{
public:
    COLORREF bkcolor;
    COLORREF boxcolor;
    point messHalfBoxPixLength;
    point messBoxSize;
    point messCenterPix;
    point clickPos;

    Mine messMap;


    game(int nr, int nc, int minnum);
    void gameInit(void);
    void gameStart();
    void gameEnd();
    void gameRender(bool gameOver = false);
    void tick();
    void gameSuccess();
    void gameFailed();
private:
    point getMessMapStartPos();
    bool click();
    bool IsInMessMap(int x, int y);
    void clearClickMsg();
    point getCurMessPosId(int x, int y);
};

game::game(int nr, int nc, int minnum)
{
    bkcolor = BROWN;
    boxcolor = BLUE;
    clickPos = point(INVAILD_ID, INVAILD_ID);
    messBoxSize = point(nr, nc);
    messHalfBoxPixLength = point(nc * BOX_WIDTH / 2, nr * BOX_WIDTH / 2);
    messCenterPix = point(WIDTH / 2, HEIGHT / 2);
    messMap = Mine(messBoxSize.x, messBoxSize.y, minnum);
    assert((nr * BOX_WIDTH < WIDTH) && (nc * BOX_WIDTH < HEIGHT));
}

void game::gameInit(void)
{
    //initgraph(width, height);
    initgraph(WIDTH, HEIGHT, EW_NOCLOSE);

    /* 初始化背景 */
    setbkcolor(bkcolor);
    cleardevice();
    /* 初始BOX颜色*/
    render::begin();
    setfillcolor(boxcolor);

    int stx = messCenterPix.x - messHalfBoxPixLength.x;
    int sty = messCenterPix.y - messHalfBoxPixLength.y;

    RECT pos = { stx,sty,stx + BOX_WIDTH,sty + BOX_WIDTH };
    for (int i = 0; i < messBoxSize.x; i++)
    {
        for (int j = 0; j < messBoxSize.y; j++)
        {
            render::drawSquare(pos.left + j * BOX_WIDTH, pos.top + i * BOX_WIDTH, BOX_WIDTH);
        }
    }

}

void game::gameStart()
{
    while (!messMap.IsInMap(clickPos))
    {
        tick();
        render::flush();
    }
    messMap.createMine(clickPos);
    do
    {
        if (messMap.IsInMap(clickPos))
        {
            if (!messMap.click(clickPos.x, clickPos.y))
            {
                gameFailed();
                break;
            }
            gameRender();
            if (messMap.IsSuccess())
            {
                gameSuccess();
                break;
            }
            clearClickMsg();
        }
        tick();
    } while (true);
}

void game::gameEnd()
{
    LPCTSTR s = _T("按任意键结束游戏!");
    render::drawMessage(s, point(messCenterPix.x - 100, messCenterPix.y + messHalfBoxPixLength.y + 50), point(200, 80));
    gameRender(true);
    (void)_getch();
}

void game::gameRender(bool gameOver)
{
    int stx = messCenterPix.x - messHalfBoxPixLength.x;
    int sty = messCenterPix.y - messHalfBoxPixLength.y;
    RECT pos = { stx,sty,stx + BOX_WIDTH,sty + BOX_WIDTH };

    for (int i = 0; i < messBoxSize.x; i++)
    {
        for (int j = 0; j < messBoxSize.y; j++)
        {
            point temp(pos.left + j * BOX_WIDTH, pos.top + i * BOX_WIDTH);
            switch (messMap.ShowMap[i][j])
            {
            case NOT_VISIT:
                if (gameOver == true && messMap.IsMine(i, j))
                {
                    render::drawBoxNum(temp, BOX_WIDTH, 0);
                }
                else
                {
                    render::drawSquare(temp.x, temp.y, BOX_WIDTH);
                }
                break;
            case VISITED:
                render::drawSquare(temp.x, temp.y, BOX_WIDTH, WHITE);
                if (messMap.getSurroundMineNum(i, j) != 0)
                {
                    break;
                }
                break;
            case VISITED_NUM:
                render::drawBoxNum(temp, BOX_WIDTH, messMap.getSurroundMineNum(i, j));
                break;
            default:
                break;
            }
        }
    }
    render::flush();
}

void game::tick()
{
    click();
}



void game::gameSuccess()
{
    LPCTSTR s = _T("恭喜通关!");
    render::drawMessage(s, point(messCenterPix.x - 100, messCenterPix.y - messHalfBoxPixLength.y - 100), point(200, 80));
    gameRender(true);
}

void game::gameFailed()
{
    LPCTSTR s = _T("通关失败!");
    render::drawMessage(s, point(messCenterPix.x - 100, messCenterPix.y - messHalfBoxPixLength.y - 100), point(200, 80));
    gameRender(true);

    int stx = messCenterPix.x - messHalfBoxPixLength.x;
    int sty = messCenterPix.y - messHalfBoxPixLength.y;
    RECT pos = { stx,sty,stx + BOX_WIDTH,sty + BOX_WIDTH };
    render::drawBoxNum(point(pos.left + clickPos.y * BOX_WIDTH, pos.top + clickPos.x * BOX_WIDTH), BOX_WIDTH, 0, RED);
    render::flush();
}

void game::clearClickMsg()
{
    clickPos = point(INVAILD_ID, INVAILD_ID);
}

point game::getMessMapStartPos()
{
    return point((messCenterPix.x - messHalfBoxPixLength.x), (messCenterPix.y - messHalfBoxPixLength.y));
}

bool game::click()
{
    MOUSEMSG m = GetMouseMsg();
    if (m.uMsg == WM_LBUTTONDOWN)
    {
        if (IsInMessMap(m.x, m.y))
        {
            clickPos = getCurMessPosId(m.x, m.y);
        }
    }
    return true;
}

bool game::IsInMessMap(int x, int y)
{
    if ((x >= (messCenterPix.x - messHalfBoxPixLength.x))
        && (x < (messCenterPix.x + messHalfBoxPixLength.x))
        && (y >= (messCenterPix.y - messHalfBoxPixLength.y))
        && (y < (messCenterPix.y + messHalfBoxPixLength.y)))
    {
        return true;
    }
    return false;
}

point game::getCurMessPosId(int x, int y)
{
    return point((y - (messCenterPix.y - messHalfBoxPixLength.y)) / BOX_WIDTH, (x - (messCenterPix.x - messHalfBoxPixLength.x)) / BOX_WIDTH);
}

int main()
{
    game gameInstance(19, 19, 59);
    gameInstance.gameInit();
    gameInstance.gameStart();
    gameInstance.gameEnd();
    render::end();
}
