#include "Game.h"

#include "ServerConfig.h"
#include "NewGameEvent.h"
#include "PlayerEliminatedEvent.h"
#include "PixelEvent.h"
#include "Utility.h"

Game::Game(std::vector<Client::SharedPtr> clients) : clients(clients) {
    gameId = RandomGenerator::getNext();
    field = std::vector<std::vector<bool>>(ServerConfig::planeWidth,
                                           std::vector<bool>(ServerConfig::planeHeight));

    auto newGameEvent = std::make_shared<NewGameEvent>(getNextEventNumber(),
                                                       ServerConfig::planeWidth,
                                                       ServerConfig::planeHeight,
                                                       getNames());
    pushEvent(newGameEvent);

    size_t number = 0;
    for (auto client : clients) {
        Position p = Position::generate();
        auto coordinates = p.round();
        if (getCell(coordinates)) {
            auto playerEliminatedEvent =
                    std::make_shared<PlayerEliminatedEvent>(
                            getNextEventNumber(),
                            number);
            pushEvent(playerEliminatedEvent);
        } else {
            setCell(coordinates);
            auto pixelEvent =
                    std::make_shared<PixelEvent>(
                            getNextEventNumber(),
                            number,
                            coordinates.first,
                            coordinates.second);
            pushEvent(pixelEvent);
            clientPositions.push_back({client, p});
        }
        number++;
    }
}

void Game::runOneTick() {
    char playerNumber = 0;
    for (auto &p : clientPositions) {
        Client::SharedPtr client = p.first;
        Position &position = p.second;
        if (position.move()) {
            auto coordinates = position.round();
            if (getCell(coordinates)) {
                auto playerEliminatedEvent =
                        std::make_shared<PlayerEliminatedEvent>(
                                getNextEventNumber(),
                                playerNumber);
                pushEvent(playerEliminatedEvent);
            } else {
                setCell(coordinates);
                auto pixelEvent =
                        std::make_shared<PixelEvent>(
                                getNextEventNumber(),
                                playerNumber,
                                coordinates.first,
                                coordinates.second);
                pushEvent(pixelEvent);
            }
        }
        playerNumber++;
    }
}

std::vector<std::string> Game::getNames() {
    std::vector<std::string> ret;
    for (auto &e : clients) {
        ret.push_back(e->getName());
    }
}

uint32_t Game::getGameId() {
    return gameId;
}

uint32_t Game::getNextEventNumber() {
    return eventNumber++;
}

bool Game::getCell(std::pair<int, int> coordinates) {
    return field[coordinates.first][coordinates.second];
}

void Game::setCell(std::pair<int, int> coordinates) {
    field[coordinates.first][coordinates.second] = true;
}

void Game::pushEvent(std::shared_ptr<Event> event) {
    events.push_back(event);
}

std::vector<std::shared_ptr<Event>> Game::getEvents(uint32_t eventNo) {
    return std::vector<std::shared_ptr<Event>>(events.begin() + eventNo, events.end());
}

void Game::turn(Client::SharedPtr client, Position::TurnDirection turn) {

}
