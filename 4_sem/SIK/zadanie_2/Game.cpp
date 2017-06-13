#include "Game.h"

#include "GameOverEvent.h"
#include "NewGameEvent.h"
#include "PixelEvent.h"
#include "PlayerEliminatedEvent.h"
#include "ServerConfig.h"

#include <iostream>

Game::Game(std::vector<Client::SharedPtr> clients,
           GameEventListener::SharedPtr listener)
    : clients(clients), listener(listener) {
    eventNumber = 0;
    gameId = RandomGenerator::getNext();
    field = std::vector<std::vector<bool>>(
        ServerConfig::planeWidth, std::vector<bool>(ServerConfig::planeHeight));
    playersLeft = clients.size();
}

void Game::start() {

    auto newGameEvent = std::make_shared<NewGameEvent>(
        getNextEventNumber(), ServerConfig::planeWidth,
        ServerConfig::planeHeight, getNames());
    pushEvent(newGameEvent);

    size_t number = 0;
    for (auto client : clients) {
        Position p = Position::generate();
        auto coordinates = p.round();
        if (getCell(coordinates)) {
            auto playerEliminatedEvent =
                std::make_shared<PlayerEliminatedEvent>(getNextEventNumber(),
                                                        number);
            client->setIsAlive(false);
            playersLeft--;
            pushEvent(playerEliminatedEvent);
        } else {
            setCell(coordinates);
            auto pixelEvent = std::make_shared<PixelEvent>(
                getNextEventNumber(), number, coordinates.first,
                coordinates.second);
            pushEvent(pixelEvent);
            clientPositions.push_back({client, p});
        }
        number++;
    }
    if (playersLeft == 1) {
        pushEvent(std::make_shared<GameOverEvent>(getNextEventNumber()));
    }
}

void Game::runOneTick() {
    char playerNumber = 0;
    for (auto &p : clientPositions) {
        Client::SharedPtr client = p.first;
        Position &position = p.second;
        if (client->isAlive()) {
            if (position.move()) {
                auto coordinates = position.round();
                if (getCell(coordinates)) {
                    auto playerEliminatedEvent =
                        std::make_shared<PlayerEliminatedEvent>(
                            getNextEventNumber(), playerNumber);
                    client->setIsAlive(false);
                    playersLeft--;
                    pushEvent(playerEliminatedEvent);
                } else {
                    setCell(coordinates);
                    auto pixelEvent = std::make_shared<PixelEvent>(
                        getNextEventNumber(), playerNumber, coordinates.first,
                        coordinates.second);
                    pushEvent(pixelEvent);
                }
            }
        }
        playerNumber++;
    }
    if (playersLeft == 1) {
        pushEvent(std::make_shared<GameOverEvent>(getNextEventNumber()));
    }
}

std::vector<std::string> Game::getNames() {
    std::vector<std::string> ret;
    for (auto &e : clients) {
        ret.push_back(e->getName());
    }
    return ret;
}

uint32_t Game::getGameId() { return gameId; }

uint32_t Game::getNextEventNumber() { return eventNumber++; }

bool Game::getCell(std::pair<int, int> coordinates) {
    if (coordinates.first < 0 || coordinates.second < 0)
        return true;
    if (coordinates.first >= ServerConfig::planeWidth)
        return true;
    if (coordinates.second >= ServerConfig::planeHeight)
        return true;
    return field[coordinates.first][coordinates.second];
}

void Game::setCell(std::pair<int, int> coordinates) {
    field[coordinates.first][coordinates.second] = true;
}

void Game::pushEvent(std::shared_ptr<Event> event) {
    std::cerr << "Pushing: " << event->toString() << std::endl;
    events.push_back(event);
    listener->onNewEvent(event);
    if (event->getEventType() == EventType::GameOver) {
        listener->onGameEnded();
    }
}

std::vector<std::shared_ptr<Event>> Game::getEvents(uint32_t eventNo) {
    std::cerr << "get events " << eventNo << std::endl;
    if (eventNo < events.size()) {
        return std::vector<std::shared_ptr<Event>>(events.begin() + eventNo,
                                                   events.end());
    } else {
        return {};
    }
}

void Game::turn(Client::SharedPtr client, Position::TurnDirection turn) {
    for (auto &e : clientPositions) {
        if (e.first == client) {
            e.second.setCurrentTurn(turn);
        }
    }
}
