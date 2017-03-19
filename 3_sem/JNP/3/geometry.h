#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cstdlib>
#include <utility>
#include <vector>

class Position;
class Vector;

using std::pair;
using std::vector;
using std::size_t;
using std::initializer_list;

class Position {
public:
    Position(int x, int y);

    int x() const;
    int y() const;

    Position reflection() const;

    bool operator==(const Position& other) const;

    Position& operator+=(const Vector& vector);

    static const Position& origin();
    static const Position& positionOfOrigin;

private:
    int _x, _y;
};

class Vector {
public:
    Vector(int x, int y);

    int x() const;
    int y() const;

    Vector reflection() const;

    bool operator==(const Vector& other) const;

    Vector& operator+=(const Vector& vector);

private:
    int _x, _y;
};

class Rectangle {
public:
    Rectangle(int width, int height, Position);
    Rectangle(int width, int height);

    int width() const;
    int height() const;
    Position pos() const;

    Rectangle reflection() const;

    long long area() const;

    pair<Rectangle, Rectangle> split_horizontally(int) const;
    pair<Rectangle, Rectangle> split_vertically(int) const;

    bool operator==(const Rectangle&) const;
    Rectangle& operator+=(const Vector&);

private:
    int _height;
    int _width;
    Position _position;
};

class Rectangles {
public:
    Rectangles(const initializer_list<Rectangle> &init);
    Rectangles();
    size_t size() const;
    void split_horizontally(size_t, int);
    void split_vertically(size_t, int);

    Rectangle& operator[](size_t);
    const Rectangle& operator[](size_t) const;
    bool operator==(const Rectangles&) const;
    Rectangles& operator+=(const Vector&);

private:
    vector<Rectangle> _rectangles;
};

/* Position + Vector */
Position operator+(Position position, const Vector& vector);
Position operator+(const Vector& vector, Position position);

/* Vector + Vector */
Vector operator+(const Vector& vector1, const Vector& vector2);
Vector&& operator+(Vector&& vector1, const Vector& vector2);
Vector&& operator+(const Vector& vector1, Vector&& vector2);
Vector&& operator+(Vector&& vector1, Vector&& vector2);

/* Merge operations */
Rectangle merge_horizontally(Rectangle rectangle1, Rectangle rectangle2);
Rectangle merge_vertically(Rectangle rectangle1, Rectangle rectangle2);

/* Rectangle + Vector */
Rectangle operator+(Rectangle rectangle, const Vector& vector);
Rectangle operator+(const Vector& vector, Rectangle rectangle);

/* Rectangles + Vector */
Rectangles operator+(Rectangles rectangles, const Vector& vector);
Rectangles operator+(const Vector& vector, Rectangles rectangles);

#endif // GEOMETRY_H
