#include "Position.h"
#include "Utility.h"
#include "ServerConfig.h"

#include <cmath>

Position Position::generate() {
    Position p;
    p.x = RandomGenerator::getNext() % ServerConfig::planeWidth + 0.5;
    p.y = RandomGenerator::getNext() % ServerConfig::planeHeight + 0.5;
    p.direction = RandomGenerator::getNext() % 360;
    return p;
}

bool Position::move() {
    turn();
    auto old = round();
    x += cos(direction / 360 * (2 * M_PI));
    y -= sin(direction / 360 * (2 * M_PI));
    return old != round();
}

void Position::turn() {
    auto d = int(currentTurn) * double(ServerConfig::turningSpeed);
    direction += d;
    direction -= (int(direction) / 360) * 360;
    if (direction < 0)
        direction = 360 - direction;
    currentTurn = TurnDirection::Straight;
}

double Position::getX() {
    return x;
}

double Position::getY() {
    return y;
}

double Position::getDirection() {
    return direction;
}

std::pair<int, int> Position::round() {
    return {(int)x, (int)y};
}

void Position::setCurrentTurn(Position::TurnDirection direction) {
    this->currentTurn = direction;
}
