#ifndef GEOMERTY_H
#define GEOMETRY_H

#include <utility>
#include <vector>
#include <cstddef>

using std::pair;
using std::vector;

class Position;
class Vector;
class Rectangle;
class Rectangles;

class Position {

public:
	Position();
	Position(int xCoordinate, int yCoordinate);

	int x() const;
	int y() const;

	Position reflection() const;

	bool operator==(const Position& other) const;

	Position& operator+=(const Vector& vector);

	static const Position& origin();

private:
	int xCoordinate, yCoordinate;

	static const Position& positionOfOrigin;
};

class Vector {

public:
	Vector();
	Vector(int xCoordinate, int yCoordinate);

	int x() const;
	int y() const;

	Vector reflection() const;

	bool operator==(const Vector& other) const;

	Vector& operator+=(const Vector& vector);

private:
	int xCoordinate, yCoordinate;
};

class Rectangle {

	int h_length;
	int w_length;
	Position position;

public:
	Rectangle(int, int, Position);
	Rectangle(int, int);

	int width() const;
	int height() const;
	Position pos() const;

	Rectangle reflection() const;

	long long area() const;

	pair<Rectangle, Rectangle> split_horizontally(int) const;
	pair<Rectangle, Rectangle> split_vertically(int) const;

	bool operator==(const Rectangle&) const;
	Rectangle& operator+=(const Vector&);
};

class Rectangles {

	vector <Rectangle> rectangles;

public:
	Rectangles(vector<Rectangle>);
	Rectangles();
	size_t size() const;
	vector <Rectangle> GetRectangles() const;
	void split_horizontally(size_t, int);
	void split_vertically(size_t, int);

	Rectangle& operator [](size_t);
	bool operator==(const Rectangles&) const;
	Rectangles& operator+=(const Vector&);
};

/* Merge operations */
Rectangle merge_horizontally(Rectangle, Rectangle);
Rectangle merge_vertically(Rectangle, Rectangle);

/* Position + Vector */
Position operator+(const Position& position, const Vector& vector);
Position operator+(const Vector& vector, const Position& position);
Position operator+(Position&& position, const Vector& vector);
Position operator+(const Vector& vector, Position&& position);

/* Vector + Vector */
Vector operator+(const Vector& vector1, const Vector& vector2);
Vector operator+(Vector&& vector1, const Vector& vector2);
Vector operator+(const Vector& vector1, Vector&& vector2);
Vector operator+(const Vector&& vector1, Vector&& vector2);

/* Rectangle + Vector */
Rectangle operator+(const Rectangle& rectangle, const Vector& vector);
Rectangle operator+(const Vector& vector, const Rectangle& rectangle);
Rectangle operator+(Rectangle&& rectangle, const Vector& vector);
Rectangle operator+(const Vector& vector, Rectangle&& rectangle);

/* Rectangles + Vector */
Rectangles operator+(const Rectangles& rectangles, const Vector& vector);
Rectangles operator+(const Vector& vector, const Rectangles& rectangles);
Rectangles operator+(Rectangles&& rectangles, const Vector& vector);
Rectangles operator+(const Vector& vector, Rectangles&& rectangles);
#endif // GEOMETRY_H
