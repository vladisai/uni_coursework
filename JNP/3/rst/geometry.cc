#include "geometry.h"
#include <iostream>
#include <utility>
using namespace std;

Position::Position(int xCoordinate, int yCoordinate)
: xCoordinate(xCoordinate)
, yCoordinate(yCoordinate)
{
}

Position::Position()
: Position(0, 0)
{
}

int Position::x() const
{
	return xCoordinate;
}

int Position::y() const
{
	return yCoordinate;
}

Position Position::reflection() const
{
	return Position(yCoordinate, xCoordinate);
}

bool Position::operator==(const Position& other) const
{
	return x() == other.x() && y() == other.y();
}

Position& Position::operator+=(const Vector& vector)
{
	xCoordinate += vector.x();
	yCoordinate += vector.y();
	return *this;
}

const Position& Position::origin()
{
	return positionOfOrigin;
}

const Position& Position::positionOfOrigin = Position();

Vector::Vector(int xCoordinate, int yCoordinate)
: xCoordinate(xCoordinate)
, yCoordinate(yCoordinate)
{
}

int Vector::x() const
{
	return xCoordinate;
}

int Vector::y() const
{
	return yCoordinate;
}

Vector Vector::reflection() const
{
	return Vector(yCoordinate, xCoordinate);
}

bool Vector::operator==(const Vector& other) const
{
	return x() == other.x() && y() == other.y();
}

Vector& Vector::operator+=(const Vector& vector)
{
	xCoordinate += vector.x();
	yCoordinate += vector.y();
	return *this;
}


//Rectangle
Rectangle::Rectangle(int x, int y)
{
	position = Position(0, 0);
	h_length = y;
	w_length = x;

}

Rectangle::Rectangle(int x, int y , Position a)
{
	position = Position(a.x(),a.y());
	h_length = x;
	w_length = y;

}


int Rectangle::width() const
{
	return w_length;
}
int Rectangle::height() const
{
	return h_length;
}

Position Rectangle::pos() const
{
	return position;
}

Rectangle Rectangle::reflection() const
{
	return Rectangle(h_length, w_length, Position(position.y(), position.x()));
}

long long Rectangle::area() const
{
	return h_length * w_length;
}


vector <Rectangle> Rectangles::GetRectangles() const
{
	return rectangles;
}

Rectangles::Rectangles()
{
	rectangles.clear();

}

pair<Rectangle, Rectangle> Rectangle::split_horizontally(int y ) const
{
	return make_pair(Rectangle(w_length, y - position.y(), Position(position.x(), position.y())),
		Rectangle(w_length, h_length - y, Position(position.x(), y))
		);

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
	return (x.height() == h_length) && (x.width() == w_length) && (x.pos() == position);
}
Rectangle& Rectangle::operator+=(const Vector& x)
{
	position += x;
	return *this;
}

Rectangles::Rectangles(vector<Rectangle> rectangles_to_put)
{
	for (size_t i = 0; i < rectangles_to_put.size(); i++)
	{
		rectangles.push_back(rectangles_to_put[i]);
	}
}


Rectangle & Rectangles:: operator[](size_t number)
{
	return rectangles[number];
}

size_t Rectangles::size() const
{
	return rectangles.size();
}

bool Rectangles:: operator==(const Rectangles &x) const
{
	if (size() != x.size())
	{
		return false;
	}
	for (size_t i = 0; i < size(); i++)
	{
		if (!(rectangles[i] == x.GetRectangles()[i]))
		{
			return false;
		}
	}
	return true;
}

void Rectangles::split_horizontally(size_t idx, int place)
{
	Rectangle x = rectangles[idx];
	rectangles.erase(rectangles.begin() + idx);
	pair<Rectangle, Rectangle> to_put = x.split_horizontally(place);
	rectangles.insert(rectangles.begin() + idx, to_put.first);
	rectangles.insert(rectangles.begin() + idx + 1, to_put.second);
}

void Rectangles::split_vertically(size_t idx, int place)
{
	Rectangle x = rectangles[idx];
	rectangles.erase(rectangles.begin() + idx);
	pair<Rectangle, Rectangle> to_put = x.split_vertically(place);
	rectangles.insert(rectangles.begin() + idx, to_put.first);
	rectangles.insert(rectangles.begin() + idx + 1, to_put.second);
}


Rectangles & Rectangles::operator+=(const Vector &vector)
{
	for (size_t i = 0; i < size(); i++)
	{
		rectangles[i] = rectangles[i] + vector;
	}
	return *this;
}



//operators


Rectangle operator+(const Rectangle& rectangle, const Vector& vector)
{
	Rectangle result = rectangle;
	result += vector;
	return Rectangle(result.width(), result.height(), Position(result.pos().x(), result.pos().y()));

}
Rectangle operator+(const Vector& vector, const Rectangle& rectangle)
{
	return rectangle + vector;
}

Rectangles operator+(const Rectangles &rectangles, const Vector &vector)
{
	Rectangles result = rectangles;
	result += vector;
	return Rectangles(result.GetRectangles());
}

Rectangles operator+(const Vector &vector, const Rectangles &rectangles)
{
	return rectangles + vector;
}


// operators

Position operator+(Position position, const Vector& vector)
{
	// position is local, so it's automatic std::move both for
	// argument and return
	return position += vector;
}

Position operator+(const Vector& vector, Position& position)
{
	return position += vector;
}

Vector operator+(const Vector& vector1, const Vector& vector2)
{
	return Vector(vector1.x() + vector2.x(), vector1.y() + vector2.y());
}

Vector operator+(Vector&& vector1, const Vector& vector2)
{
	vector1 += vector2;
	return std::move(vector1);
}

Vector operator+(const Vector& vector1, Vector&& vector2)
{
	vector2 += vector1;
	return std::move(vector2);
}

Vector operator+(Vector&& vector1, Vector&& vector2)
{
	vector1 += vector2;
	return std::move(vector1);
}
