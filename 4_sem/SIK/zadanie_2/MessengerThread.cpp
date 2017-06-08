#include "MessengerThread.h"
#include "ServerConnection.h"

void startMessageReceiverThread(std::weak_ptr<GameManager> manager) {
    std::thread t(messageReceiverLoop, manager);
    t.detach();
}

void messageReceiverLoop(std::weak_ptr<GameManager> manager) {
    ServerConnection connection(ServerConfig::portNumber);
    while (true) {
        auto addressMessagePair = connection.receive();
        auto managerPtr = manager.lock();
        if (managerPtr) {
            managerPtr->addMessage(addressMessagePair);
        }
    }
}
