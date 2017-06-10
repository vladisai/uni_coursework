#include "MessageLoops.h"

#include <thread>

void MessageLoops::runReceiverLoop(GameManager::SharedPtr manager,
                            ServerConnection::SharedPtr connection) {
    std::thread([=]() {
        while (true) {
            try {
                auto e = connection->receive();
                manager->addMessage(e);
            } catch (...) {}
        }
    }).detach();
}
