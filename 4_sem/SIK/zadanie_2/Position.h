#ifndef NETACKA_SIK_POSITION_H
#define NETACKA_SIK_POSITION_H

#include <utility>

class Position {
public:
    static Position generate();

    bool move(); // returns true if rounded coordinates changed

    double getX();
    double getY();
    double getDirection();

    std::pair<int, int> round();

    enum TurnDirection {
        Left = 1,
        Straight = 0,
        Right = -1
    };

    void setCurrentTurn(TurnDirection direction);

private:

    void turn();

    TurnDirection currentTurn;
    double x, y, direction;
};

#endif //NETACKA_SIK_POSITION_H
