#ifndef GW1K_POINT_H_
#define GW1K_POINT_H_

#include <ostream>

namespace gw1k
{

class Point
{

public:

    Point();

    Point(int x, int y);

    Point(const Point& other);

    ~Point();

public:

    Point& operator+=(const Point& rhs);

    Point& operator-=(const Point& rhs);

    const Point operator+(const Point& other) const;

    const Point operator-(const Point& other) const;

    const Point operator-() const;

    friend std::ostream& operator<<(std::ostream& out, const Point& point);

    friend const Point min(const Point& p1, const Point& p2);

    friend const Point max(const Point& p1, const Point& p2);

public:

    int x;

    int y;

};

} // namespace gw1k

#endif // GW1K_POINT2D_H_
