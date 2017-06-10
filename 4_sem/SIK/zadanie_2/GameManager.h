#ifndef NETACKA_SIK_GAMEMANAGER_H
#define NETACKA_SIK_GAMEMANAGER_H

#include <mutex>
#include <chrono>

#include "Client.h"
#include "Game.h"
#include "ServerConnection.h"
#include "Utility.h"

class GameManager
        : public std::enable_shared_from_this<GameManager>,
          public GameEventListener
{
public:
    using SharedPtr = std::shared_ptr<GameManager>;

    GameManager(ServerConnection::SharedPtr connection);

    void addMessage(ServerConnection::ClientAddressMessagePair message);

    void run();

private:
    std::mutex bufferMutex;
    std::vector<ServerConnection::ClientAddressMessagePair> messagesBuffer;
    std::vector<Client::SharedPtr> connectedClients;
    Game::SharedPtr currentGame;
    Clock::time_point lastIterationTime;
    ServerConnection::SharedPtr connection;

    bool isGameGoing;

    void cleanDisconnectedClients();

    void processMessages();

    void processMessage(const ServerConnection::ClientAddressMessagePair &addressMessagePair);

    void tryStartGame();

    bool canStartGame();

    void markIterationTime();

    void waitUntilNextIteration();

    void mainLoop();

    void sendEvents(ServerConnection::ClientAddress address,
                                 std::vector<Event::SharedPtr> events);

    virtual void onGameEnded() override;

    virtual void onNewEvent(Event::SharedPtr event) override;

};

#endif //NETACKA_SIK_GAMEMANAGER_H
