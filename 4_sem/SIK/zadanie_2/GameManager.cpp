#include <algorithm>
#include <chrono>
#include <iostream>

#include "GameManager.h"
#include "ServerConfig.h"
#include "CommonConfig.h"
#include "MessageLoops.h"
#include "CommonConfig.h"

GameManager::GameManager(ServerConnection::SharedPtr connection) : connection(connection), isGameGoing(false) {

}

void GameManager::cleanDisconnectedClients() {
    for (auto &e : connectedClients) {
        if (e->isTimedOut()) {
            e->setIsActive(false);
        }
    }
    connectedClients.erase(
            std::remove_if(connectedClients.begin(),
                           connectedClients.end(),
                           [](const Client::SharedPtr &c) { return !c->isActive(); }),
            connectedClients.end()
    );
}

void GameManager::processMessages() {
    std::cerr << "GameManager: got " << messagesBuffer.size() << " messages" << std::endl;
    std::lock_guard <std::mutex> lock(bufferMutex);
    for (auto &messageAddressPair : messagesBuffer) {
        processMessage(messageAddressPair);
    }
    messagesBuffer.clear();
}

void GameManager::processMessage(const ServerConnection::ClientAddressMessagePair &addressMessagePair) {
    ServerConnection::ClientAddress address = addressMessagePair.first;
    ClientMessage message = addressMessagePair.second;
    std::cerr << "GameManager: processing message " << message.toString() << std::endl;
    auto client = std::find_if(connectedClients.begin(), connectedClients.end(),
                               [address](Client::SharedPtr ptr) { return address == ptr->getAddress(); });
    if (client == connectedClients.end()) {
        // create and connect if there are less than 42 players
        if (connectedClients.size() < ServerConfig::maxClientsNumber) {
            if (message.getPlayerName() != "") {
                // check for name duplicates
                for (auto e : connectedClients) {
                    if (e->getName() == message.getPlayerName())
                        return; // ignore, someone is trying to steal an identity
                }
            }
            auto newClient = std::make_shared<Client>(address, message.getSessionId(),
                                                      message.getPlayerName(), message.getNextExpectedEventNo());
            connectedClients.push_back(newClient);
            client = connectedClients.end() - 1;
        } else {
            return; // we just ignore other clients if we have too much to do
        }
    } else if ((*client)->getSessionId() != message.getSessionId()) {
        if ((*client)->getSessionId() < message.getSessionId()) {
            // update info
            auto newClient = std::make_shared<Client>(address, message.getSessionId(),
                                                      message.getPlayerName(), message.getNextExpectedEventNo());
            *client = newClient;
        } else {
            return; // ignore past session messages
        }
    }

    (*client)->updateActiveTime();
    if (!isGameGoing && message.getTurnDirection() != Position::TurnDirection::Straight) {
        std::cerr << "GameManager: turn registered!" << std::endl;
        (*client)->setIsReady(true);
    }
    if (currentGame) {
        sendEvents(address, currentGame->getEvents(message.getNextExpectedEventNo()));
    }
    if ((*client)->isPlaying()) {
        currentGame->turn(*client, (Position::TurnDirection) message.getTurnDirection());
    }
}

void GameManager::tryStartGame() {
    std::cerr << "GameManager: trying to start" << std::endl;
    if (canStartGame()) {
        isGameGoing = true;
        std::vector <Client::SharedPtr> players;
        std::copy_if(connectedClients.begin(), connectedClients.end(), std::back_inserter(players),
                     [](const Client::SharedPtr &ptr) { return !(ptr->getName().empty()) && ptr->isReady(); });
        for (auto &e : players) {
            e->setIsPlaying(true);
            e->setIsReady(false);
            e->setIsAlive(true);
        }
        sort(players.begin(), players.end(), [](const Client::SharedPtr &p1,
                                                const Client::SharedPtr &p2) {
            return p1->getName() < p2->getName();
        });
        auto me = shared_from_this();
        currentGame = std::make_shared<Game>(players, me);
        currentGame->start();
        std::cerr << "GameManager: game started!" << std::endl;
    } else {
        std::cerr << "GameManager: couldn't start" << std::endl;
    }
}

bool GameManager::canStartGame() {
    if (isGameGoing) {
        return false;
    }
    int count = 0;
    std::cerr << "GameManager: connected clients number: " << connectedClients.size() << std::endl;
    for (auto client : connectedClients) {
        if (!client->getName().empty() && !client->isReady()) {
            return false;
        }
        if (!client->getName().empty()) {
            count++;
        }
    }
    if (count < CommonConfig::minPlayersNumber) {
        return false;
    }
    return true;
}

void GameManager::mainLoop() {
    while (!isStopping) {
        waitUntilNextIteration();
        std::cerr << "GameManager: main loop iteration started" << std::endl;
        markIterationTime();
        cleanDisconnectedClients();
        processMessages();
        if (isGameGoing) {
            std::cerr << "GameManager: Game going!" << std::endl;
            currentGame->runOneTick();
        } else {
            tryStartGame();
        }
    }
}

void GameManager::waitUntilNextIteration() {
    auto current = Clock::now();
    auto next = lastIterationTime + ServerConfig::gameSpeed;
    sleepUntil(next);
}

void GameManager::markIterationTime() {
    lastIterationTime = Clock::now();
}

void GameManager::addMessage(ServerConnection::ClientAddressMessagePair messagePair) {
    std::lock_guard <std::mutex> lock(bufferMutex);
    messagesBuffer.push_back(messagePair);
}

void GameManager::onGameEnded() {
    isGameGoing = false;
    tryStartGame();
}

void GameManager::onNewEvent(Event::SharedPtr event) {
    for (auto client : connectedClients) {
        connection->send(client->getAddress(), ServerMessage(currentGame->getGameId(), {event}));
    }
}

void GameManager::sendEvents(ServerConnection::ClientAddress address,
                             std::vector<Event::SharedPtr> events) {
    if (events.size() > 0) {
        std::cerr << "GameManager: seding events " << events.size() << std::endl;
        auto rawMessages = compressMessages(currentGame->getGameId(), events);
        for (auto &e : events) {
            std::cerr << e->toString() << std::endl;
        }
        for (auto &e : rawMessages) {
            connection->send(address, e);
        }
    }
}

void GameManager::run() {
    messageThread = MessageLoops::runReceiverLoop(shared_from_this(), connection);
    mainLoop();
}

void GameManager::stop() {
    isStopping = true;
    messageThread.join();
}

bool GameManager::shouldTerminate() {
    return isStopping;
}
