#ifndef POINT_H
#define POINT_H

class Point {
    
    public:

        Point();
        Point(int xCoordinate, int yCoordinate);

        int x() const;
        int y() const;

        Point reflection() const;

        bool operator == (const Point& other) const;

        //Point& operator += (const Vector& vector); 

    private:

        int xCoordinate, yCoordinate;

};
#endif
