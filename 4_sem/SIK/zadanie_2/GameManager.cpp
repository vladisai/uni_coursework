#include "GameManager.h"

GameManager::GameManager() {
    ServerConnection::launchMessageReceivingThread(std::weak_ptr<GameManager>(this));
    mainLoop();
}

void GameManager::onGameEnd() {
    tryStartGame();
}

void GameManager::onNewEvent(std::shared_ptr <Event> event) {
    for (auto client : connectedClients) {
        // TODO: send event to the client
    }
}

void GameManager::cleanDisconnectedClients() {
    connectedClients.erase(
            std::remove_if(connectedClients.begin(),
                           connectedClients.end(),
                           [](const Client &c) { return c.isDisconnected(); })
    );
}

void GameManager::processMessages() {
    std::lock_guard <std::mutex> lock(bufferMutex);
    for (auto &messageAddressPair : messagesBuffer) {
        processMessage(messageAddressPair);
    }
    messagesBuffer.clear();
}

void GameManager::processMessage(const ClientAddressMessagePair &messageAddressPair) {
    ClientMessage &message = messageAddressPair.first;
    ClientAddress &address = messageAddressPair.second;
    auto client = std::find_if(connectedClients.begin(), connectedClients.end(),
                               [](Client::SharedPtr ptr) { return ptr->getAddress() == address; });
    if (client == connectedClients.end()) {
        client.setIsPlaying(false);
        connectedClients.push_back(make_shared<Client>(client));
    } else if (client->getSessionId() != message.getSessionId()) {
        client->setSessionId(message.getSessionId());
        client->setIsPlaying(false);
    } else {
        sendEvents(address, currentGame->getEvents(message.getNextExpectedEventNo()));
        if (client->isPlaying()) {
            currentGame->turn(client, message.getTurnDirection());
        }
    }
}

void GameManager::tryStartGame() {
    if (canStartGame()) {
        isGameGoing = true;
        std::vector <Client::SharedPtr> players;
        std::copy_if(connectedClients.begin(), connectedClients.end(), std::back_inserter(players),
                     [](const Client::SharedPtr &ptr) { return !ptr->getName().empty(); });
        currentGame = std::make_shared<Game>(connectedClients);
    }
}

bool GameManager::canStartGame() {
    if (!isGameGoing) {
        return false;
    }
    for (auto client : connectedClients) {
        if (client.getName() != "" && !client.isReady()) {
            return false;
        }
    }
    if (connectedClients.size() < 2) {
        return false;
    }
    return true;
}

void GameManager::mainLoop() {
    while (true) {
        while (!shouldStartNextIteration());
        markIterationTime();
        processNewMessages();
        if (isGameGoing) {
            currentGame->runOneTick();
        } else {
            tryStartGame();
        }
    }
}

bool GameManager::shouldStartNextIteration() {
    auto current = Clock::now();
    auto interval = current - lastIterationTime;
    return (interval.count() > ServerConfig::gameSpeed);
}

void GameManager::markIterationTime() {
    lastIterationTime = Clock::now();
}

void GameManager::addMessage(ClientMessage message) {
    std::lock_guard <std::mutex> lock(bufferMutex);
    messagesBuffer.push_back(message);
}
