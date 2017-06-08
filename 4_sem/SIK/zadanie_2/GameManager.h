#ifndef NETACKA_SIK_GAMEMANAGER_H
#define NETACKA_SIK_GAMEMANAGER_H

#include <mutex>

#include "Client.h"
#include "Game.h"
#include "ServerConnection.h"

class GameManager {
public:
    GameManager();

    void onGameEnd();

    void onNewEvent();

    void addMessage(ClientAddressMessagePair message);

private:
    std::mutex bufferMutex;
    std::vector<ClientAddressMessagePair> messagesBuffer;
    std::vector<Client::SharedPtr> connectedClients;
    std::shared_ptr<Game> currentGame;

    bool isGameGoing;

    void cleanDisconnectedClients();

    void processMessages();

    void processMessage(const ClientAddressMessagePair &messagePair);

    void tryStartGame();

    bool canStartGame();

    void markIterationTime();

    bool shouldStartNextIteration();

};

#endif //NETACKA_SIK_GAMEMANAGER_H
