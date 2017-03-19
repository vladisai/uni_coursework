#include <cassert>
#include "geometry.h"

Position::Position(int x, int y)
    : _x(x)
    , _y(y)
{
}

int Position::x() const
{
    return _x;
}

int Position::y() const
{
    return _y;
}

Position Position::reflection() const
{
    return Position(_y, _x);
}

bool Position::operator==(const Position& other) const
{
    return x() == other.x() && y() == other.y();
}

Position& Position::operator+=(const Vector& vector)
{
    _x += vector.x();
    _y += vector.y();
    return *this;
}

const Position& Position::origin()
{
    return positionOfOrigin;
}

const Position& Position::positionOfOrigin = Position(0, 0);

Vector::Vector(int x, int y)
    : _x(x)
    , _y(y)
{
}

int Vector::x() const
{
    return _x;
}

int Vector::y() const
{
    return _y;
}

Vector Vector::reflection() const
{
    return Vector(_y, _x);
}

bool Vector::operator==(const Vector& other) const
{
    return x() == other.x() && y() == other.y();
}

Vector& Vector::operator+=(const Vector& vector)
{
    _x += vector.x();
    _y += vector.y();
    return *this;
}

//Rectangle
Rectangle::Rectangle(int width, int height)
    : Rectangle(width, height, Position(0, 0))
{
}

Rectangle::Rectangle(int width, int height, Position position)
    : _height(height)
    , _width(width)
    , _position(position)
{
    assert(height > 0);
    assert(width > 0);
}

int Rectangle::width() const
{
    return _width;
}

int Rectangle::height() const
{
    return _height;
}

Position Rectangle::pos() const
{
    return _position;
}

Rectangle Rectangle::reflection() const
{
    return Rectangle(_height, _width, _position.reflection());
}

long long Rectangle::area() const
{
    return _height * _width;
}

Rectangles::Rectangles()
{
}

Rectangles::Rectangles(const initializer_list<Rectangle>& init)
    : _rectangles(init.begin(), init.end())
{
}

pair<Rectangle, Rectangle> Rectangle::split_horizontally(int y) const
{
    return std::make_pair(Rectangle(_width, y, _position),
        Rectangle(_width, _height - y, Position(_position.x(), _position.y() + y)));
}

pair<Rectangle, Rectangle> Rectangle::split_vertically(int x) const
{
    pair<Rectangle, Rectangle> ans = reflection().split_horizontally(x);
    ans.first = ans.first.reflection();
    ans.second = ans.second.reflection();
    return ans;
}

bool Rectangle::operator==(const Rectangle& x) const
{
    return (x.height() == _height) && (x.width() == _width) && (x.pos() == _position);
}

Rectangle& Rectangle::operator+=(const Vector& x)
{
    _position += x;
    return *this;
}

Rectangle& Rectangles::operator[](size_t number)
{
    return _rectangles[number];
}

const Rectangle& Rectangles::operator[](size_t number) const
{
    return _rectangles[number];
}

size_t Rectangles::size() const
{
    return _rectangles.size();
}

bool Rectangles::operator==(const Rectangles& x) const
{
    return _rectangles == x._rectangles;
}

void Rectangles::split_horizontally(size_t idx, int place)
{
    Rectangle x = _rectangles[idx];
    _rectangles.erase(_rectangles.begin() + idx);
    pair<Rectangle, Rectangle> to_put = x.split_horizontally(place);
    _rectangles.insert(_rectangles.begin() + idx, to_put.first);
    _rectangles.insert(_rectangles.begin() + idx + 1, to_put.second);
}

void Rectangles::split_vertically(size_t idx, int place)
{
    Rectangle x = _rectangles[idx];
    _rectangles.erase(_rectangles.begin() + idx);
    pair<Rectangle, Rectangle> to_put = x.split_vertically(place);
    _rectangles.insert(_rectangles.begin() + idx, to_put.first);
    _rectangles.insert(_rectangles.begin() + idx + 1, to_put.second);
}

Rectangles& Rectangles::operator+=(const Vector& vector)
{
    for (size_t i = 0; i < size(); i++) {
        _rectangles[i] += vector;
    }
    return *this;
}

// rectangle + vector

Rectangle operator+(Rectangle rectangle, const Vector& vector)
{
    // rectangle is local, so it does std::move automatically both for
    // argument and returned value if rectangle is an rval ref
    rectangle += vector;
    return rectangle;
}

Rectangle operator+(const Vector& vector, Rectangle rectangle)
{
    return rectangle + vector;
}

// rectangles + vector

Rectangles operator+(Rectangles rectangles, const Vector& vector)
{
    // rectangles is local, so it does std::move automatically both for
    // argument and returned value if rectangle is an rval ref
    rectangles += vector;
    return rectangles;
}

Rectangles operator+(const Vector& vector, Rectangles rectangles)
{
    return rectangles + vector;
}

// position + vector

Position operator+(Position position, const Vector& vector)
{
    // position is local, so it does std::move automatically both for
    // argument and returned value if rectangle is an rval ref
    return position += vector;
}

Position operator+(const Vector& vector, Position position)
{
    return position + vector;
}

// vector + vector

Vector operator+(const Vector& vector1, const Vector& vector2)
{
    return Vector(vector1.x() + vector2.x(), vector1.y() + vector2.y());
}

Vector&& operator+(Vector&& vector1, const Vector& vector2)
{
    vector1 += vector2;
    return std::move(vector1);
}

Vector&& operator+(const Vector& vector1, Vector&& vector2)
{
    vector2 += vector1;
    return std::move(vector2);
}

Vector&& operator+(Vector&& vector1, Vector&& vector2)
{
    vector1 += vector2;
    return std::move(vector1);
}

//merge

Rectangle merge_horizontally(Rectangle a, Rectangle b)
{
    if (a.pos().y() > b.pos().y())
        return merge_vertically(b, a);
    assert(a.width() == b.width());
    assert(a.pos().x() == b.pos().x());
    assert(a.pos().y() + a.height() == b.pos().y());
    return Rectangle(a.width(), a.height() + b.height(), Position(a.pos().x(), a.pos().y()));
}

Rectangle merge_vertically(Rectangle a, Rectangle b)
{
    if (a.pos().x() > b.pos().x())
        return merge_vertically(b, a);
    assert(a.height() == b.height());
    assert(a.pos().y() == b.pos().y());
    assert(a.pos().x() + a.width() == b.pos().x());
    return Rectangle(a.width() + b.width(), a.height(), Position(a.pos().x(), a.pos().y()));
}
