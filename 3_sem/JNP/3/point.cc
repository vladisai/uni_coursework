#include "point.h"

Point::Point(int xCoordinate, int yCoordinate)
    : xCoordinate(xCoordinate)
    , yCoordinate(yCoordinate)
{
}

Point::Point()
    : Point(0, 0)
{
}

int Point::x() const {
    return xCoordinate;
}

int Point::y() const {
    return yCoordinate;
}

Point Point::reflection() const {
    return Point(yCoordinate, xCoordinate);
}


bool Point::operator==(const Point& other) const
{
    return x() == other.x() && y() == other.y();
}

/*Point& Point::operator += (const Vector& vector) {
    return Point(xCoordinate + vector.x(), yCoordinate + vector.y());
}*/
