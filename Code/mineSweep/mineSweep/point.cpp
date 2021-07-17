#include <iostream>
#include "point.h"

point::point(int _x /*= 0*/, int _y /*= 0*/) : x(_x), y(_y) {}

point::point(const point& p)
{
    x = p.x;
    y = p.y;
}


point point::operator+(point& other)
{
    int nx = x + other.x;
    int ny = y + other.y;
    return point(nx, ny);
}

std::ostream& operator<<(std::ostream& out, const point& p)
{
    out << "(" << p.x << "," << p.y << ")" << std::endl;
    return out;
}
