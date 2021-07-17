#pragma once

class point
{
public:
    int x;
    int y;

    point(int _x = 0, int _y = 0);

    point(const point& p);

    point operator+(point& other);

    friend std::ostream& operator<<(std::ostream& out, const point& p);
};
