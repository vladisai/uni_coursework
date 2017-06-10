#ifndef NETACKA_SIK_GAME_H
#define NETACKA_SIK_GAME_H

#include <chrono>

#include "Client.h"
#include "Position.h"

class GameEventListener {
public:
    using SharedPtr = std::shared_ptr<GameEventListener>;
    virtual void onGameEnded() = 0;
    virtual void onNewEvent(Event::SharedPtr event) = 0;
};

class Game {
public:
    using SharedPtr = std::shared_ptr<Game>;
    Game(std::vector<Client::SharedPtr> clients, GameEventListener::SharedPtr listener);

    std::vector<std::shared_ptr<Event>> getEvents(uint32_t nextId = 0);

    uint32_t getGameId();

    void runOneTick();

    void start();

    void turn(Client::SharedPtr client, Position::TurnDirection turn);

private:

    using Clock = std::chrono::system_clock;
    using Time = std::chrono::time_point<Clock>;

    uint32_t gameId;
    Time lastIterationTime;

    std::vector<std::string> getNames();

    std::vector<std::vector<bool>> field;
    std::vector<Event::SharedPtr> events;
    std::vector<Client::SharedPtr> clients;
    std::vector<std::pair<Client::SharedPtr, Position>> clientPositions;
    uint32_t eventNumber;
    uint32_t playersLeft;

    GameEventListener::SharedPtr listener;

    uint32_t getNextEventNumber();

    bool getCell(std::pair<int, int> coordinates);
    void setCell(std::pair<int, int> coordinates);

    void pushEvent(std::shared_ptr<Event> event);

};

#endif //NETACKA_SIK_GAME_H
