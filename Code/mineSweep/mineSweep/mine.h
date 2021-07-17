#pragma once
#include "common_type.h"
#include "point.h"
#include <vector>

#define DEFAULT_ROW_NUM     (9)
#define DEFAULT_COL_NUM     (9)
#define DEFAULT_MINE_NUM    (9)

#define EMPTY_FLAG          (0)
#define MINE_FLAG           (1)

#define NOT_VISIT           (0)
#define VISITED             (1)
#define VISITED_NUM         (2)

#define INVAILD_ID          (0xFFFF)

class Mine
{
private:
    int mineNum;
    std::vector<std::vector<u8_t>> mineMap;
    std::vector<std::vector<u8_t>> mineSurroundCount;
public:
    int row;
    int col;
    std::vector<std::vector<u8_t>> ShowMap;

    Mine(int nr = DEFAULT_ROW_NUM, int nc = DEFAULT_COL_NUM, int num = DEFAULT_MINE_NUM);

    void createMine(point p = point(INVAILD_ID, INVAILD_ID));

    u8_t getMineVal(int ir, int ic);
    u8_t getMineVal(point p);

    u8_t getSurroundMineNum(int ir, int ic);
    u8_t getSurroundMineNum(point p);

    bool IsMine(int ir, int ic);
    bool IsMine(point p);

    bool IsInMap(int ir, int ic);
    bool IsInMap(point p);

    void updateShowMap(int r, int c);

    bool click(int r, int c);

    bool IsSuccess();

    void showMine();

    friend std::ostream& operator<<(std::ostream& out, Mine& myMine);
private:
    void CalcSurroundMineNum();
};
